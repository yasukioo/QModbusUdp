// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbServerHandler.h>

#include <QModbusUdp/MbAduCodec.h>
#include <QModbusUdp/MbException.h>

namespace QMbUdp {
namespace {

void appendU16(QByteArray &bytes, quint16 value)
{
    bytes.append(char((value >> 8) & 0xff));
    bytes.append(char(value & 0xff));
}

quint16 readU16(const QByteArray &bytes, int offset)
{
    return (quint16(quint8(bytes.at(offset))) << 8) | quint16(quint8(bytes.at(offset + 1)));
}

QByteArray holdingPayload(const QVector<quint16> &values)
{
    QByteArray payload;
    payload.append(char(values.size() * 2));
    for (quint16 value : values) {
        appendU16(payload, value);
    }
    return payload;
}

QByteArray coilsPayload(const QBitArray &bits)
{
    const int byteCount = (bits.size() + 7) / 8;
    QByteArray payload;
    payload.append(char(byteCount));
    payload.append(QByteArray(byteCount, char(0)));
    for (int i = 0; i < bits.size(); ++i) {
        if (bits.testBit(i)) {
            payload[1 + i / 8] = char(quint8(payload.at(1 + i / 8)) | (1 << (i % 8)));
        }
    }
    return payload;
}

} // namespace

QByteArray MbServerHandler::handleDatagram(const QByteArray &adu, MbDataModel *model) const
{
    try {
        const MbFrame frame = MbAduCodec::decode(adu);
        const quint8 rawFunction = quint8(frame.pdu.function);
        const FunctionCode function = FunctionCode(rawFunction);

        if (!model) {
            return MbAduCodec::encodeException(
                frame.transactionId, frame.unitId, function, ExceptionCode::ServerDeviceFailure);
        }

        if (function == FunctionCode::ReadHoldingRegisters) {
            if (frame.pdu.payload.size() != 4) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            QVector<quint16> values;
            if (!model->readHolding(readU16(frame.pdu.payload, 0), readU16(frame.pdu.payload, 2), values)) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataAddress);
            }
            return MbAduCodec::encodeResponse(frame.transactionId, frame.unitId,
                                              {function, holdingPayload(values)});
        }

        if (function == FunctionCode::ReadCoils) {
            if (frame.pdu.payload.size() != 4) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            QBitArray bits;
            if (!model->readCoils(readU16(frame.pdu.payload, 0), readU16(frame.pdu.payload, 2), bits)) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataAddress);
            }
            return MbAduCodec::encodeResponse(frame.transactionId, frame.unitId,
                                              {function, coilsPayload(bits)});
        }

        if (function == FunctionCode::WriteSingleCoil) {
            if (frame.pdu.payload.size() != 4) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            const quint16 address = readU16(frame.pdu.payload, 0);
            const quint16 encodedValue = readU16(frame.pdu.payload, 2);
            if (encodedValue != 0xff00 && encodedValue != 0x0000) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            if (!model->writeCoils(address, {encodedValue == 0xff00})) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataAddress);
            }
            return MbAduCodec::encodeResponse(frame.transactionId, frame.unitId,
                                              {function, frame.pdu.payload});
        }

        if (function == FunctionCode::WriteMultipleRegisters) {
            if (frame.pdu.payload.size() < 5) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            const quint16 address = readU16(frame.pdu.payload, 0);
            const quint16 count = readU16(frame.pdu.payload, 2);
            const int byteCount = quint8(frame.pdu.payload.at(4));
            if (byteCount != count * 2 || frame.pdu.payload.size() != 5 + byteCount) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataValue);
            }
            QVector<quint16> values;
            values.reserve(count);
            for (int i = 0; i < count; ++i) {
                values.append(readU16(frame.pdu.payload, 5 + i * 2));
            }
            if (!model->writeHolding(address, values)) {
                return MbAduCodec::encodeException(
                    frame.transactionId, frame.unitId, function, ExceptionCode::IllegalDataAddress);
            }
            QByteArray payload;
            appendU16(payload, address);
            appendU16(payload, count);
            return MbAduCodec::encodeResponse(frame.transactionId, frame.unitId, {function, payload});
        }

        return MbAduCodec::encodeException(
            frame.transactionId, frame.unitId, function, ExceptionCode::IllegalFunction);
    } catch (const MbProtocolException &) {
        return {};
    }
}

} // namespace QMbUdp

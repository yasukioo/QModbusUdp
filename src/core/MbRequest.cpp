// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbRequest.h>

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

void validateEcho(const MbPdu &pdu, FunctionCode expected, int payloadSize)
{
    if (pdu.function != expected || pdu.payload.size() != payloadSize) {
        throw MbProtocolException(QStringLiteral("Unexpected response PDU"));
    }
}

} // namespace

MbReadCoilsRequest::MbReadCoilsRequest(quint16 address, quint16 quantity)
    : address_(address)
    , quantity_(quantity)
{
}

FunctionCode MbReadCoilsRequest::functionCode() const { return FunctionCode::ReadCoils; }

MbPdu MbReadCoilsRequest::encodePdu() const
{
    QByteArray payload;
    appendU16(payload, address_);
    appendU16(payload, quantity_);
    return {functionCode(), payload};
}

MbResponse MbReadCoilsRequest::decodeResponse(const MbPdu &pdu) const
{
    if (pdu.function != functionCode() || pdu.payload.isEmpty()) {
        throw MbProtocolException(QStringLiteral("Unexpected read coils response"));
    }
    const int byteCount = quint8(pdu.payload.at(0));
    if (pdu.payload.size() != 1 + byteCount) {
        throw MbProtocolException(QStringLiteral("Invalid read coils byte count"));
    }

    QBitArray bits(quantity_);
    for (int i = 0; i < quantity_; ++i) {
        const int byteIndex = 1 + i / 8;
        const int bitIndex = i % 8;
        bits.setBit(i, quint8(pdu.payload.at(byteIndex)) & (1 << bitIndex));
    }
    return MbResponse::coils(bits);
}

QSharedPointer<MbRequest> MbReadCoilsRequest::clone() const
{
    return QSharedPointer<MbRequest>(new MbReadCoilsRequest(*this));
}

quint16 MbReadCoilsRequest::address() const { return address_; }
quint16 MbReadCoilsRequest::quantity() const { return quantity_; }

MbReadHoldingRegRequest::MbReadHoldingRegRequest(quint16 address, quint16 quantity)
    : address_(address)
    , quantity_(quantity)
{
}

FunctionCode MbReadHoldingRegRequest::functionCode() const
{
    return FunctionCode::ReadHoldingRegisters;
}

MbPdu MbReadHoldingRegRequest::encodePdu() const
{
    QByteArray payload;
    appendU16(payload, address_);
    appendU16(payload, quantity_);
    return {functionCode(), payload};
}

MbResponse MbReadHoldingRegRequest::decodeResponse(const MbPdu &pdu) const
{
    if (pdu.function != functionCode() || pdu.payload.isEmpty()) {
        throw MbProtocolException(QStringLiteral("Unexpected read holding response"));
    }
    const int byteCount = quint8(pdu.payload.at(0));
    if (byteCount != quantity_ * 2 || pdu.payload.size() != 1 + byteCount) {
        throw MbProtocolException(QStringLiteral("Invalid read holding byte count"));
    }

    QVector<quint16> values;
    values.reserve(quantity_);
    for (int i = 0; i < quantity_; ++i) {
        values.append(readU16(pdu.payload, 1 + i * 2));
    }
    return MbResponse::holdingRegisters(values);
}

QSharedPointer<MbRequest> MbReadHoldingRegRequest::clone() const
{
    return QSharedPointer<MbRequest>(new MbReadHoldingRegRequest(*this));
}

quint16 MbReadHoldingRegRequest::address() const { return address_; }
quint16 MbReadHoldingRegRequest::quantity() const { return quantity_; }

MbWriteSingleCoilRequest::MbWriteSingleCoilRequest(quint16 address, bool value)
    : address_(address)
    , value_(value)
{
}

FunctionCode MbWriteSingleCoilRequest::functionCode() const
{
    return FunctionCode::WriteSingleCoil;
}

MbPdu MbWriteSingleCoilRequest::encodePdu() const
{
    QByteArray payload;
    appendU16(payload, address_);
    appendU16(payload, value_ ? 0xff00 : 0x0000);
    return {functionCode(), payload};
}

MbResponse MbWriteSingleCoilRequest::decodeResponse(const MbPdu &pdu) const
{
    validateEcho(pdu, functionCode(), 4);
    return MbResponse::acknowledgeWrite(readU16(pdu.payload, 0), readU16(pdu.payload, 2));
}

QSharedPointer<MbRequest> MbWriteSingleCoilRequest::clone() const
{
    return QSharedPointer<MbRequest>(new MbWriteSingleCoilRequest(*this));
}

quint16 MbWriteSingleCoilRequest::address() const { return address_; }
bool MbWriteSingleCoilRequest::value() const { return value_; }

MbWriteMultipleRegRequest::MbWriteMultipleRegRequest(quint16 address, QVector<quint16> values)
    : address_(address)
    , values_(std::move(values))
{
}

FunctionCode MbWriteMultipleRegRequest::functionCode() const
{
    return FunctionCode::WriteMultipleRegisters;
}

MbPdu MbWriteMultipleRegRequest::encodePdu() const
{
    QByteArray payload;
    appendU16(payload, address_);
    appendU16(payload, quint16(values_.size()));
    payload.append(char(values_.size() * 2));
    for (quint16 value : values_) {
        appendU16(payload, value);
    }
    return {functionCode(), payload};
}

MbResponse MbWriteMultipleRegRequest::decodeResponse(const MbPdu &pdu) const
{
    validateEcho(pdu, functionCode(), 4);
    return MbResponse::acknowledgeWrite(readU16(pdu.payload, 0), readU16(pdu.payload, 2));
}

QSharedPointer<MbRequest> MbWriteMultipleRegRequest::clone() const
{
    return QSharedPointer<MbRequest>(new MbWriteMultipleRegRequest(*this));
}

quint16 MbWriteMultipleRegRequest::address() const { return address_; }
QVector<quint16> MbWriteMultipleRegRequest::values() const { return values_; }

} // namespace QMbUdp

// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbAduCodec.h>

#include <QModbusUdp/MbException.h>
#include <QModbusUdp/MbRequest.h>

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

QByteArray encodeAdu(Tid tid, UnitId unitId, quint8 function, const QByteArray &payload)
{
    QByteArray bytes;
    bytes.reserve(8 + payload.size());
    appendU16(bytes, tid);
    appendU16(bytes, 0);
    appendU16(bytes, quint16(1 + 1 + payload.size()));
    bytes.append(char(unitId));
    bytes.append(char(function));
    bytes.append(payload);
    return bytes;
}

} // namespace

QByteArray MbAduCodec::encodeRequest(Tid tid, UnitId unitId, const MbRequest &request)
{
    const MbPdu pdu = request.encodePdu();
    return encodeAdu(tid, unitId, quint8(pdu.function), pdu.payload);
}

QByteArray MbAduCodec::encodeResponse(Tid tid, UnitId unitId, const MbPdu &pdu)
{
    return encodeAdu(tid, unitId, quint8(pdu.function), pdu.payload);
}

QByteArray MbAduCodec::encodeException(Tid tid,
                                       UnitId unitId,
                                       FunctionCode function,
                                       ExceptionCode exception)
{
    QByteArray payload;
    payload.append(char(quint8(exception)));
    return encodeAdu(tid, unitId, quint8(function) | 0x80, payload);
}

MbFrame MbAduCodec::decode(const QByteArray &adu)
{
    if (adu.size() < 8) {
        throw MbProtocolException(QStringLiteral("ADU is shorter than MBAP header plus function code"));
    }

    MbFrame frame;
    frame.transactionId = readU16(adu, 0);
    frame.protocolId = readU16(adu, 2);
    const quint16 length = readU16(adu, 4);
    frame.unitId = quint8(adu.at(6));

    if (frame.protocolId != 0) {
        throw MbProtocolException(QStringLiteral("MBAP protocol id must be zero"));
    }
    if (length < 2) {
        throw MbProtocolException(QStringLiteral("MBAP length must include unit id and function code"));
    }
    if (adu.size() != 6 + length) {
        throw MbProtocolException(QStringLiteral("MBAP length does not match ADU size"));
    }

    frame.pdu.function = FunctionCode(quint8(adu.at(7)));
    frame.pdu.payload = adu.mid(8);
    return frame;
}

} // namespace QMbUdp

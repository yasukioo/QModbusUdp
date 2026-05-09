// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QByteArray>

#include <QModbusUdp/MbResponse.h>
#include <QModbusUdp/MbTypes.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class MbRequest;

struct QMBUDP_EXPORT MbPdu {
    FunctionCode function = FunctionCode::ReadHoldingRegisters;
    QByteArray payload;
};

struct QMBUDP_EXPORT MbFrame {
    Tid transactionId = 0;
    quint16 protocolId = 0;
    UnitId unitId = 0;
    MbPdu pdu;
};

class QMBUDP_EXPORT MbAduCodec {
public:
    static QByteArray encodeRequest(Tid tid, UnitId unitId, const MbRequest &request);
    static QByteArray encodeResponse(Tid tid, UnitId unitId, const MbPdu &pdu);
    static QByteArray encodeException(Tid tid,
                                      UnitId unitId,
                                      FunctionCode function,
                                      ExceptionCode exception);
    static MbFrame decode(const QByteArray &adu);
};

} // namespace QMbUdp

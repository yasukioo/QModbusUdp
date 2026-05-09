// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QBitArray>
#include <QVector>

#include <QModbusUdp/MbTypes.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbResponse {
public:
    static MbResponse coils(QBitArray bits);
    static MbResponse holdingRegisters(QVector<quint16> values);
    static MbResponse acknowledgeWrite(quint16 address, quint16 value);
    static MbResponse exception(FunctionCode function, ExceptionCode code);

    bool isException() const;
    FunctionCode functionCode() const;
    ExceptionCode exceptionCode() const;
    QBitArray coils() const;
    QVector<quint16> holdingRegisters() const;
    quint16 address() const;
    quint16 value() const;

private:
    FunctionCode function_ = FunctionCode::ReadHoldingRegisters;
    ExceptionCode exception_ = ExceptionCode::IllegalFunction;
    bool isException_ = false;
    QBitArray coils_;
    QVector<quint16> holding_;
    quint16 address_ = 0;
    quint16 value_ = 0;
};

} // namespace QMbUdp

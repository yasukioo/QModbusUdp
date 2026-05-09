// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbResponse.h>

namespace QMbUdp {

MbResponse MbResponse::coils(QBitArray bits)
{
    MbResponse response;
    response.function_ = FunctionCode::ReadCoils;
    response.coils_ = std::move(bits);
    return response;
}

MbResponse MbResponse::holdingRegisters(QVector<quint16> values)
{
    MbResponse response;
    response.function_ = FunctionCode::ReadHoldingRegisters;
    response.holding_ = std::move(values);
    return response;
}

MbResponse MbResponse::acknowledgeWrite(quint16 address, quint16 value)
{
    MbResponse response;
    response.function_ = FunctionCode::WriteSingleCoil;
    response.address_ = address;
    response.value_ = value;
    return response;
}

MbResponse MbResponse::exception(FunctionCode function, ExceptionCode code)
{
    MbResponse response;
    response.function_ = function;
    response.exception_ = code;
    response.isException_ = true;
    return response;
}

bool MbResponse::isException() const { return isException_; }
FunctionCode MbResponse::functionCode() const { return function_; }
ExceptionCode MbResponse::exceptionCode() const { return exception_; }
QBitArray MbResponse::coils() const { return coils_; }
QVector<quint16> MbResponse::holdingRegisters() const { return holding_; }
quint16 MbResponse::address() const { return address_; }
quint16 MbResponse::value() const { return value_; }

} // namespace QMbUdp

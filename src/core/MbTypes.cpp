// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbTypes.h>

namespace QMbUdp {

QString functionCodeName(FunctionCode code)
{
    switch (code) {
    case FunctionCode::ReadCoils: return QStringLiteral("ReadCoils");
    case FunctionCode::ReadDiscreteInputs: return QStringLiteral("ReadDiscreteInputs");
    case FunctionCode::ReadHoldingRegisters: return QStringLiteral("ReadHoldingRegisters");
    case FunctionCode::ReadInputRegisters: return QStringLiteral("ReadInputRegisters");
    case FunctionCode::WriteSingleCoil: return QStringLiteral("WriteSingleCoil");
    case FunctionCode::WriteSingleRegister: return QStringLiteral("WriteSingleRegister");
    case FunctionCode::WriteMultipleCoils: return QStringLiteral("WriteMultipleCoils");
    case FunctionCode::WriteMultipleRegisters: return QStringLiteral("WriteMultipleRegisters");
    }
    return QStringLiteral("Unknown");
}

QString exceptionCodeName(ExceptionCode code)
{
    switch (code) {
    case ExceptionCode::IllegalFunction: return QStringLiteral("IllegalFunction");
    case ExceptionCode::IllegalDataAddress: return QStringLiteral("IllegalDataAddress");
    case ExceptionCode::IllegalDataValue: return QStringLiteral("IllegalDataValue");
    case ExceptionCode::ServerDeviceFailure: return QStringLiteral("ServerDeviceFailure");
    }
    return QStringLiteral("Unknown");
}

QString errorName(Error error)
{
    switch (error) {
    case Error::NoError: return QStringLiteral("NoError");
    case Error::Timeout: return QStringLiteral("Timeout");
    case Error::NetworkError: return QStringLiteral("NetworkError");
    case Error::ProtocolError: return QStringLiteral("ProtocolError");
    case Error::ExceptionResponse: return QStringLiteral("ExceptionResponse");
    }
    return QStringLiteral("Unknown");
}

} // namespace QMbUdp

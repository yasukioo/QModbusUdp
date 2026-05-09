// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QMetaType>
#include <QString>

#include <QtGlobal>

namespace QMbUdp {

using Tid = quint16;
using UnitId = quint8;

enum class FunctionCode : quint8 {
    ReadCoils = 0x01,
    ReadDiscreteInputs = 0x02,
    ReadHoldingRegisters = 0x03,
    ReadInputRegisters = 0x04,
    WriteSingleCoil = 0x05,
    WriteSingleRegister = 0x06,
    WriteMultipleCoils = 0x0f,
    WriteMultipleRegisters = 0x10,
};

enum class ExceptionCode : quint8 {
    IllegalFunction = 0x01,
    IllegalDataAddress = 0x02,
    IllegalDataValue = 0x03,
    ServerDeviceFailure = 0x04,
};

enum class Error {
    NoError,
    Timeout,
    NetworkError,
    ProtocolError,
    ExceptionResponse,
};

QString functionCodeName(FunctionCode code);
QString exceptionCodeName(ExceptionCode code);
QString errorName(Error error);

} // namespace QMbUdp

Q_DECLARE_METATYPE(QMbUdp::FunctionCode)
Q_DECLARE_METATYPE(QMbUdp::Error)

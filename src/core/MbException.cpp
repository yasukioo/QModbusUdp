// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbException.h>

namespace QMbUdp {

MbProtocolException::MbProtocolException(QString message, ExceptionCode code)
    : message_(std::move(message))
    , code_(code)
{
}

void MbProtocolException::raise() const
{
    throw *this;
}

MbProtocolException *MbProtocolException::clone() const
{
    return new MbProtocolException(*this);
}

QString MbProtocolException::message() const
{
    return message_;
}

ExceptionCode MbProtocolException::exceptionCode() const
{
    return code_;
}

} // namespace QMbUdp

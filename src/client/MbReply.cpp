// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbReply.h>

namespace QMbUdp {

MbReply::MbReply(QObject *parent)
    : QObject(parent)
{
}

bool MbReply::isFinished() const { return finished_; }
Error MbReply::error() const { return error_; }
QString MbReply::errorString() const { return errorString_; }
MbResponse MbReply::response() const { return response_; }

void MbReply::finishWithResponse(const MbResponse &response)
{
    if (finished_) {
        return;
    }
    response_ = response;
    error_ = response.isException() ? Error::ExceptionResponse : Error::NoError;
    if (response.isException()) {
        errorString_ = exceptionCodeName(response.exceptionCode());
    }
    finished_ = true;
    emit finished();
}

void MbReply::finishWithError(Error error, const QString &message)
{
    if (finished_) {
        return;
    }
    error_ = error;
    errorString_ = message;
    finished_ = true;
    emit finished();
}

} // namespace QMbUdp

// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QObject>
#include <QString>

#include <QModbusUdp/MbResponse.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbReply : public QObject {
    Q_OBJECT

public:
    explicit MbReply(QObject *parent = nullptr);

    bool isFinished() const;
    Error error() const;
    QString errorString() const;
    MbResponse response() const;

    void finishWithResponse(const MbResponse &response);
    void finishWithError(Error error, const QString &message);

signals:
    void finished();

private:
    bool finished_ = false;
    Error error_ = Error::NoError;
    QString errorString_;
    MbResponse response_;
};

} // namespace QMbUdp

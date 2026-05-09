// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QHostAddress>
#include <QObject>
#include <QScopedPointer>

#include <QModbusUdp/MbReply.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbRetryPolicy.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class MbUdpClientPrivate;

class QMBUDP_EXPORT MbUdpClient : public QObject {
    Q_OBJECT

public:
    explicit MbUdpClient(QObject *parent = nullptr);
    ~MbUdpClient() override;

    bool bind(const QHostAddress &address = QHostAddress::AnyIPv4, quint16 port = 0);
    void setDefaultTimeout(int timeoutMs);
    void setMaxRetries(int retries);
    void setRetryPolicy(const MbRetryPolicy &policy);

    MbReply *send(const MbRequest &request,
                  UnitId unitId,
                  const QHostAddress &peer,
                  quint16 port);

signals:
    void replyFinished(QMbUdp::MbReply *reply);

private:
    QScopedPointer<MbUdpClientPrivate> d_;
};

} // namespace QMbUdp

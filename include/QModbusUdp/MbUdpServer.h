// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QScopedPointer>

#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/MbServerHandler.h>
#include <QModbusUdp/MbTypes.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class MbUdpServerPrivate;

class QMBUDP_EXPORT MbUdpServer : public QObject {
    Q_OBJECT

public:
    explicit MbUdpServer(QObject *parent = nullptr);
    ~MbUdpServer() override;

    bool listen(const QHostAddress &address = QHostAddress::AnyIPv4, quint16 port = 502);
    quint16 localPort() const;
    MbDataModel *dataModel(UnitId unitId);

signals:
    void requestReceived(QMbUdp::UnitId unitId, QMbUdp::FunctionCode functionCode);

private:
    QScopedPointer<MbUdpServerPrivate> d_;
};

} // namespace QMbUdp

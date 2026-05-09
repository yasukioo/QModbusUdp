// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QBitArray>
#include <QObject>
#include <QVector>

#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbDataModel : public QObject {
    Q_OBJECT

public:
    explicit MbDataModel(QObject *parent = nullptr);

    bool readHolding(quint16 address, quint16 count, QVector<quint16> &out) const;
    bool writeHolding(quint16 address, const QVector<quint16> &values);

    bool readCoils(quint16 address, quint16 count, QBitArray &out) const;
    bool writeCoils(quint16 address, const QVector<bool> &values);
    bool writeCoils(quint16 address, std::initializer_list<bool> values);

signals:
    void holdingChanged(int address, int count);
    void coilsChanged(int address, int count);

private:
    QVector<quint16> holding_;
    QBitArray coils_;
};

} // namespace QMbUdp

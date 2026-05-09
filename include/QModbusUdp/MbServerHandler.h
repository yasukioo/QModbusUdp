// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QByteArray>

#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbServerHandler {
public:
    QByteArray handleDatagram(const QByteArray &adu, MbDataModel *model) const;
};

} // namespace QMbUdp

// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QByteArray>
#include <QHostAddress>
#include <QString>

#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbLogger {
public:
    enum class Level { Trace, Debug, Info, Warn, Error };
    enum class Direction { Tx, Rx };

    virtual ~MbLogger() = default;

    virtual void log(Level level, const QString &context, const QString &message) = 0;
    virtual void trace(Direction direction,
                       const QHostAddress &peer,
                       quint16 port,
                       const QByteArray &adu);
};

} // namespace QMbUdp

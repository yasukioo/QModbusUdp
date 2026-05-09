// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QException>
#include <QString>

#include <QModbusUdp/MbTypes.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbProtocolException final : public QException {
public:
    explicit MbProtocolException(QString message,
                                 ExceptionCode code = ExceptionCode::IllegalDataValue);

    void raise() const override;
    MbProtocolException *clone() const override;

    QString message() const;
    ExceptionCode exceptionCode() const;

private:
    QString message_;
    ExceptionCode code_;
};

} // namespace QMbUdp

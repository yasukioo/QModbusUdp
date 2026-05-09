// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QtGlobal>

#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbRetryPolicy {
public:
    enum class Backoff { Linear, Exponential };

    int timeoutMs() const;
    int maxRetries() const;
    int baseDelayMs() const;
    int capDelayMs() const;
    Backoff backoff() const;

    void setTimeoutMs(int timeoutMs);
    void setMaxRetries(int maxRetries);
    void setBaseDelayMs(int baseDelayMs);
    void setCapDelayMs(int capDelayMs);
    void setBackoff(Backoff backoff);

    int delayFor(int attempt) const;

private:
    int timeoutMs_ = 1000;
    int maxRetries_ = 3;
    int baseDelayMs_ = 200;
    int capDelayMs_ = 5000;
    Backoff backoff_ = Backoff::Linear;
};

} // namespace QMbUdp

// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbRetryPolicy.h>

#include <QtMath>

namespace QMbUdp {

int MbRetryPolicy::timeoutMs() const { return timeoutMs_; }
int MbRetryPolicy::maxRetries() const { return maxRetries_; }
int MbRetryPolicy::baseDelayMs() const { return baseDelayMs_; }
int MbRetryPolicy::capDelayMs() const { return capDelayMs_; }
MbRetryPolicy::Backoff MbRetryPolicy::backoff() const { return backoff_; }

void MbRetryPolicy::setTimeoutMs(int timeoutMs) { timeoutMs_ = qMax(1, timeoutMs); }
void MbRetryPolicy::setMaxRetries(int maxRetries) { maxRetries_ = qMax(0, maxRetries); }
void MbRetryPolicy::setBaseDelayMs(int baseDelayMs) { baseDelayMs_ = qMax(0, baseDelayMs); }
void MbRetryPolicy::setCapDelayMs(int capDelayMs) { capDelayMs_ = qMax(1, capDelayMs); }
void MbRetryPolicy::setBackoff(Backoff backoff) { backoff_ = backoff; }

int MbRetryPolicy::delayFor(int attempt) const
{
    const int normalizedAttempt = qMax(1, attempt);
    if (backoff_ == Backoff::Exponential) {
        return qMin(baseDelayMs_ * (1 << (normalizedAttempt - 1)), capDelayMs_);
    }
    return qMin(baseDelayMs_ * normalizedAttempt, capDelayMs_);
}

} // namespace QMbUdp

// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#pragma once

#include <QByteArray>
#include <QSharedPointer>
#include <QVector>

#include <QModbusUdp/MbAduCodec.h>
#include <QModbusUdp/MbResponse.h>
#include <QModbusUdp/qmodbusudp_global.h>

namespace QMbUdp {

class QMBUDP_EXPORT MbRequest {
public:
    virtual ~MbRequest() = default;

    virtual FunctionCode functionCode() const = 0;
    virtual MbPdu encodePdu() const = 0;
    virtual MbResponse decodeResponse(const MbPdu &pdu) const = 0;
    virtual QSharedPointer<MbRequest> clone() const = 0;
};

class QMBUDP_EXPORT MbReadCoilsRequest final : public MbRequest {
public:
    MbReadCoilsRequest(quint16 address, quint16 quantity);

    FunctionCode functionCode() const override;
    MbPdu encodePdu() const override;
    MbResponse decodeResponse(const MbPdu &pdu) const override;
    QSharedPointer<MbRequest> clone() const override;

    quint16 address() const;
    quint16 quantity() const;

private:
    quint16 address_;
    quint16 quantity_;
};

class QMBUDP_EXPORT MbReadHoldingRegRequest final : public MbRequest {
public:
    MbReadHoldingRegRequest(quint16 address, quint16 quantity);

    FunctionCode functionCode() const override;
    MbPdu encodePdu() const override;
    MbResponse decodeResponse(const MbPdu &pdu) const override;
    QSharedPointer<MbRequest> clone() const override;

    quint16 address() const;
    quint16 quantity() const;

private:
    quint16 address_;
    quint16 quantity_;
};

class QMBUDP_EXPORT MbWriteSingleCoilRequest final : public MbRequest {
public:
    MbWriteSingleCoilRequest(quint16 address, bool value);

    FunctionCode functionCode() const override;
    MbPdu encodePdu() const override;
    MbResponse decodeResponse(const MbPdu &pdu) const override;
    QSharedPointer<MbRequest> clone() const override;

    quint16 address() const;
    bool value() const;

private:
    quint16 address_;
    bool value_;
};

class QMBUDP_EXPORT MbWriteMultipleRegRequest final : public MbRequest {
public:
    MbWriteMultipleRegRequest(quint16 address, QVector<quint16> values);

    FunctionCode functionCode() const override;
    MbPdu encodePdu() const override;
    MbResponse decodeResponse(const MbPdu &pdu) const override;
    QSharedPointer<MbRequest> clone() const override;

    quint16 address() const;
    QVector<quint16> values() const;

private:
    quint16 address_;
    QVector<quint16> values_;
};

} // namespace QMbUdp

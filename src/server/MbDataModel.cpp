// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbDataModel.h>

namespace QMbUdp {

MbDataModel::MbDataModel(QObject *parent)
    : QObject(parent)
    , holding_(65536)
    , coils_(65536)
{
}

bool MbDataModel::readHolding(quint16 address, quint16 count, QVector<quint16> &out) const
{
    if (count == 0 || int(address) + int(count) > holding_.size()) {
        return false;
    }
    out = holding_.mid(address, count);
    return true;
}

bool MbDataModel::writeHolding(quint16 address, const QVector<quint16> &values)
{
    if (values.isEmpty() || int(address) + values.size() > holding_.size()) {
        return false;
    }
    for (int i = 0; i < values.size(); ++i) {
        holding_[address + i] = values.at(i);
    }
    emit holdingChanged(address, values.size());
    return true;
}

bool MbDataModel::readCoils(quint16 address, quint16 count, QBitArray &out) const
{
    if (count == 0 || int(address) + int(count) > coils_.size()) {
        return false;
    }
    out.resize(count);
    for (int i = 0; i < count; ++i) {
        out.setBit(i, coils_.testBit(address + i));
    }
    return true;
}

bool MbDataModel::writeCoils(quint16 address, const QVector<bool> &values)
{
    if (values.isEmpty() || int(address) + values.size() > coils_.size()) {
        return false;
    }
    for (int i = 0; i < values.size(); ++i) {
        coils_.setBit(address + i, values.at(i));
    }
    emit coilsChanged(address, values.size());
    return true;
}

bool MbDataModel::writeCoils(quint16 address, std::initializer_list<bool> values)
{
    QVector<bool> vector;
    vector.reserve(int(values.size()));
    for (bool value : values) {
        vector.append(value);
    }
    return writeCoils(address, vector);
}

} // namespace QMbUdp

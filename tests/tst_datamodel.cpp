// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbDataModel.h>

#include <QtTest/QtTest>

using namespace QMbUdp;

class DataModelTest : public QObject {
    Q_OBJECT

private slots:
    void storesHoldingRegisters();
    void reportsOutOfRangeAccess();
    void storesCoilsAndEmitsChangeSignal();
};

void DataModelTest::storesHoldingRegisters()
{
    MbDataModel model;
    QVERIFY(model.writeHolding(10, {0x1111, 0x2222, 0x3333}));

    QVector<quint16> values;
    QVERIFY(model.readHolding(10, 3, values));
    QCOMPARE(values, QVector<quint16>({0x1111, 0x2222, 0x3333}));
}

void DataModelTest::reportsOutOfRangeAccess()
{
    MbDataModel model;
    QVector<quint16> values;

    QVERIFY(!model.readHolding(65535, 2, values));
    QVERIFY(!model.writeHolding(65535, {1, 2}));
}

void DataModelTest::storesCoilsAndEmitsChangeSignal()
{
    MbDataModel model;
    QSignalSpy spy(&model, &MbDataModel::coilsChanged);

    QVERIFY(model.writeCoils(4, {true, false, true}));

    QBitArray bits;
    QVERIFY(model.readCoils(4, 3, bits));
    QCOMPARE(bits.size(), 3);
    QVERIFY(bits.testBit(0));
    QVERIFY(!bits.testBit(1));
    QVERIFY(bits.testBit(2));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 4);
}

QTEST_MAIN(DataModelTest)
#include "tst_datamodel.moc"

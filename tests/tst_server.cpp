// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbServerHandler.h>

#include <QtTest/QtTest>

using namespace QMbUdp;

class ServerTest : public QObject {
    Q_OBJECT

private slots:
    void handlesReadHoldingRegisters();
    void handlesWriteSingleCoil();
    void rejectsUnknownFunctionCode();
};

void ServerTest::handlesReadHoldingRegisters()
{
    MbDataModel model;
    QVERIFY(model.writeHolding(100, {0x1234, 0x5678}));
    MbServerHandler handler;

    const QByteArray req = MbAduCodec::encodeRequest(4, 1, MbReadHoldingRegRequest(100, 2));
    const QByteArray resp = handler.handleDatagram(req, &model);

    QCOMPARE(resp.toHex(' '), QByteArray("00 04 00 00 00 07 01 03 04 12 34 56 78"));
}

void ServerTest::handlesWriteSingleCoil()
{
    MbDataModel model;
    MbServerHandler handler;

    const QByteArray req = MbAduCodec::encodeRequest(5, 1, MbWriteSingleCoilRequest(8, true));
    const QByteArray resp = handler.handleDatagram(req, &model);

    QCOMPARE(resp.toHex(' '), QByteArray("00 05 00 00 00 06 01 05 00 08 ff 00"));

    QBitArray bits;
    QVERIFY(model.readCoils(8, 1, bits));
    QVERIFY(bits.testBit(0));
}

void ServerTest::rejectsUnknownFunctionCode()
{
    MbDataModel model;
    MbServerHandler handler;
    const QByteArray req = QByteArray::fromHex("0006000000020141");

    const QByteArray resp = handler.handleDatagram(req, &model);

    QCOMPARE(resp.toHex(' '), QByteArray("00 06 00 00 00 03 01 c1 01"));
}

QTEST_MAIN(ServerTest)
#include "tst_server.moc"

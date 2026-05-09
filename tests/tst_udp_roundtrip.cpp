// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbReply.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbUdpClient.h>
#include <QModbusUdp/MbUdpServer.h>

#include <QtTest/QtTest>

using namespace QMbUdp;

class UdpRoundtripTest : public QObject {
    Q_OBJECT

private slots:
    void clientReadsHoldingRegistersFromLocalServer();
};

void UdpRoundtripTest::clientReadsHoldingRegistersFromLocalServer()
{
    MbUdpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));
    QVERIFY(server.localPort() > 0);
    QVERIFY(server.dataModel(1)->writeHolding(100, {0x1234, 0x5678}));

    MbUdpClient client;
    QVERIFY(client.bind(QHostAddress::LocalHost, 0));
    client.setDefaultTimeout(200);
    client.setMaxRetries(1);

    MbReadHoldingRegRequest request(100, 2);
    MbReply *reply = client.send(request, 1, QHostAddress::LocalHost, server.localPort());
    QSignalSpy spy(reply, &MbReply::finished);

    QVERIFY(spy.wait(1000));
    QCOMPARE(reply->error(), Error::NoError);
    QCOMPARE(reply->response().holdingRegisters(), QVector<quint16>({0x1234, 0x5678}));
}

QTEST_MAIN(UdpRoundtripTest)
#include "tst_udp_roundtrip.moc"

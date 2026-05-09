// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbException.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbTypes.h>

#include <QtTest/QtTest>

using namespace QMbUdp;

class CodecTest : public QObject {
    Q_OBJECT

private slots:
    void encodesReadHoldingRegistersRequestAdu();
    void decodesReadHoldingRegistersResponse();
    void rejectsMalformedMbapLength();
    void encodesExceptionResponse();
};

void CodecTest::encodesReadHoldingRegistersRequestAdu()
{
    MbReadHoldingRegRequest request(100, 2);

    const QByteArray adu = MbAduCodec::encodeRequest(0x1234, 0x01, request);

    QCOMPARE(adu.toHex(' '), QByteArray("12 34 00 00 00 06 01 03 00 64 00 02"));
}

void CodecTest::decodesReadHoldingRegistersResponse()
{
    const QByteArray adu = QByteArray::fromHex("12340000000701030412345678");

    const MbFrame frame = MbAduCodec::decode(adu);
    QCOMPARE(frame.transactionId, 0x1234);
    QCOMPARE(frame.unitId, 0x01);
    QCOMPARE(frame.pdu.function, FunctionCode::ReadHoldingRegisters);

    const MbReadHoldingRegRequest request(100, 2);
    const MbResponse response = request.decodeResponse(frame.pdu);

    QCOMPARE(response.holdingRegisters(), QVector<quint16>({0x1234, 0x5678}));
}

void CodecTest::rejectsMalformedMbapLength()
{
    const QByteArray adu = QByteArray::fromHex("12340000000601030412345678");

    QVERIFY_THROWS_EXCEPTION(MbProtocolException, MbAduCodec::decode(adu));
}

void CodecTest::encodesExceptionResponse()
{
    const QByteArray adu = MbAduCodec::encodeException(
        7,
        2,
        FunctionCode::ReadCoils,
        ExceptionCode::IllegalDataAddress);

    QCOMPARE(adu.toHex(' '), QByteArray("00 07 00 00 00 03 02 81 02"));
}

QTEST_MAIN(CodecTest)
#include "tst_codec.moc"

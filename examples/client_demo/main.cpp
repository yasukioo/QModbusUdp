// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbReply.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbUdpClient.h>
#include <QTimer>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpClient client;
    client.bind();
    client.setDefaultTimeout(800);
    client.setMaxRetries(3);

    MbReadHoldingRegRequest request(100, 10);
    MbReply *reply = client.send(request, 1, QHostAddress(QStringLiteral("127.0.0.1")), 1502);

    QObject::connect(reply, &MbReply::finished, reply, [reply] {
        if (reply->error() == Error::NoError) {
            qInfo() << "holding[100..109] =" << reply->response().holdingRegisters();
        } else {
            qWarning() << "err:" << reply->errorString();
        }
        reply->deleteLater();
        QCoreApplication::quit();
    });

    return app.exec();
}

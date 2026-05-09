// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/MbUdpServer.h>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpServer server;
    if (!server.listen(QHostAddress::AnyIPv4, 1502)) {
        qCritical() << "listen failed";
        return 1;
    }

    MbDataModel *model = server.dataModel(1);
    model->writeHolding(100, {0x1234, 0x5678, 0xaa55, 1, 2, 3, 4, 5, 6, 7});

    QObject::connect(model, &MbDataModel::coilsChanged, model, [model](int address, int count) {
        QBitArray bits;
        model->readCoils(address, count, bits);
        qInfo() << "coils changed" << address << count << bits;
    });
    QObject::connect(&server, &MbUdpServer::requestReceived, &server,
                     [](UnitId unitId, FunctionCode code) {
                         qDebug() << "unit" << unitId << "FC" << int(code);
                     });

    return app.exec();
}

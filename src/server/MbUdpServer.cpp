// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbUdpServer.h>

#include <QModbusUdp/MbAduCodec.h>
#include <QModbusUdp/MbException.h>

#include <QNetworkDatagram>
#include <QUdpSocket>

namespace QMbUdp {

class MbUdpServerPrivate {
public:
    explicit MbUdpServerPrivate(MbUdpServer *owner)
        : q(owner)
        , socket(new QUdpSocket(owner))
    {
    }

    MbUdpServer *q = nullptr;
    QUdpSocket *socket = nullptr;
    MbServerHandler handler;
    QHash<UnitId, MbDataModel *> models;

    void onReadyRead()
    {
        while (socket->hasPendingDatagrams()) {
            QNetworkDatagram datagram = socket->receiveDatagram();
            MbFrame frame;
            try {
                frame = MbAduCodec::decode(datagram.data());
            } catch (const MbProtocolException &) {
                continue;
            }

            MbDataModel *model = models.value(frame.unitId, nullptr);
            const QByteArray response = handler.handleDatagram(datagram.data(), model);
            emit q->requestReceived(frame.unitId, frame.pdu.function);
            if (!response.isEmpty()) {
                socket->writeDatagram(response, datagram.senderAddress(), datagram.senderPort());
            }
        }
    }
};

MbUdpServer::MbUdpServer(QObject *parent)
    : QObject(parent)
    , d_(new MbUdpServerPrivate(this))
{
    connect(d_->socket, &QUdpSocket::readyRead, this, [this] { d_->onReadyRead(); });
}

MbUdpServer::~MbUdpServer() = default;

bool MbUdpServer::listen(const QHostAddress &address, quint16 port)
{
    return d_->socket->bind(address, port);
}

quint16 MbUdpServer::localPort() const
{
    return d_->socket->localPort();
}

MbDataModel *MbUdpServer::dataModel(UnitId unitId)
{
    auto it = d_->models.find(unitId);
    if (it != d_->models.end()) {
        return it.value();
    }
    auto *model = new MbDataModel(this);
    d_->models.insert(unitId, model);
    return model;
}

} // namespace QMbUdp

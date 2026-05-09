// Copyright (c) 2026 yasukioo
// Author: yasukioo <yasukioo@outlook.com>

#include <QModbusUdp/MbUdpClient.h>

#include <QModbusUdp/MbAduCodec.h>
#include <QModbusUdp/MbException.h>

#include <QHash>
#include <QNetworkDatagram>
#include <QPointer>
#include <QQueue>
#include <QTimer>
#include <QUdpSocket>

#include <optional>

namespace QMbUdp {
namespace {

struct SessionKey {
    QHostAddress peer;
    quint16 port = 0;
    UnitId unitId = 0;

    bool operator==(const SessionKey &other) const
    {
        return peer == other.peer && port == other.port && unitId == other.unitId;
    }
};

uint qHash(const SessionKey &key, uint seed = 0)
{
    return ::qHash(key.peer.toString(), seed)
        ^ ::qHash(key.port, seed)
        ^ ::qHash(key.unitId, seed);
}

struct QueueItem {
    Tid tid = 0;
    QByteArray adu;
    QSharedPointer<MbRequest> request;
    QPointer<MbReply> reply;
    int attempt = 0;
};

struct Session {
    QQueue<QueueItem> pending;
    std::optional<QueueItem> inflight;
    Tid nextTid = 1;
    QTimer *timer = nullptr;
};

} // namespace

class MbUdpClientPrivate {
public:
    explicit MbUdpClientPrivate(MbUdpClient *owner)
        : q(owner)
        , socket(new QUdpSocket(owner))
    {
    }

    MbUdpClient *q = nullptr;
    QUdpSocket *socket = nullptr;
    MbRetryPolicy policy;
    QHash<SessionKey, Session *> sessions;

    Session *sessionFor(const SessionKey &key)
    {
        auto it = sessions.find(key);
        if (it != sessions.end()) {
            return it.value();
        }

        auto *session = new Session;
        session->timer = new QTimer(q);
        session->timer->setSingleShot(true);
        QObject::connect(session->timer, &QTimer::timeout, q, [this, key] { onTimeout(key); });
        sessions.insert(key, session);
        return session;
    }

    void pump(const SessionKey &key)
    {
        Session *session = sessionFor(key);
        if (session->inflight || session->pending.isEmpty()) {
            return;
        }

        session->inflight = session->pending.dequeue();
        sendInflight(key);
    }

    void sendInflight(const SessionKey &key)
    {
        Session *session = sessionFor(key);
        if (!session->inflight) {
            return;
        }

        QueueItem &item = *session->inflight;
        ++item.attempt;
        const qint64 sent = socket->writeDatagram(item.adu, key.peer, key.port);
        if (sent != item.adu.size()) {
            if (item.reply) {
                item.reply->finishWithError(Error::NetworkError, socket->errorString());
                emit q->replyFinished(item.reply);
            }
            session->inflight.reset();
            pump(key);
            return;
        }
        session->timer->start(policy.timeoutMs());
    }

    void onTimeout(const SessionKey &key)
    {
        Session *session = sessionFor(key);
        if (!session->inflight) {
            return;
        }

        QueueItem &item = *session->inflight;
        if (item.attempt <= policy.maxRetries()) {
            QTimer::singleShot(policy.delayFor(item.attempt), q, [this, key] { sendInflight(key); });
            return;
        }

        if (item.reply) {
            item.reply->finishWithError(Error::Timeout, QStringLiteral("Modbus UDP request timed out"));
            emit q->replyFinished(item.reply);
        }
        session->inflight.reset();
        pump(key);
    }

    void onReadyRead()
    {
        while (socket->hasPendingDatagrams()) {
            QHostAddress peer;
            quint16 port = 0;
            const QByteArray adu = socket->receiveDatagram().data();
            Q_UNUSED(peer);
            Q_UNUSED(port);

            MbFrame frame;
            try {
                frame = MbAduCodec::decode(adu);
            } catch (const MbProtocolException &) {
                continue;
            }

            for (auto it = sessions.begin(); it != sessions.end(); ++it) {
                Session *session = it.value();
                if (!session->inflight || session->inflight->tid != frame.transactionId) {
                    continue;
                }

                session->timer->stop();
                QueueItem item = *session->inflight;
                session->inflight.reset();
                if (item.reply) {
                    try {
                        const quint8 raw = quint8(frame.pdu.function);
                        if (raw & 0x80) {
                            const auto original = FunctionCode(raw & 0x7f);
                            const auto code = frame.pdu.payload.isEmpty()
                                ? ExceptionCode::ServerDeviceFailure
                                : ExceptionCode(quint8(frame.pdu.payload.at(0)));
                            item.reply->finishWithResponse(MbResponse::exception(original, code));
                        } else {
                            item.reply->finishWithResponse(item.request->decodeResponse(frame.pdu));
                        }
                    } catch (const MbProtocolException &e) {
                        item.reply->finishWithError(Error::ProtocolError, e.message());
                    }
                    emit q->replyFinished(item.reply);
                }
                pump(it.key());
                break;
            }
        }
    }
};

MbUdpClient::MbUdpClient(QObject *parent)
    : QObject(parent)
    , d_(new MbUdpClientPrivate(this))
{
    connect(d_->socket, &QUdpSocket::readyRead, this, [this] { d_->onReadyRead(); });
}

MbUdpClient::~MbUdpClient() = default;

bool MbUdpClient::bind(const QHostAddress &address, quint16 port)
{
    return d_->socket->bind(address, port);
}

void MbUdpClient::setDefaultTimeout(int timeoutMs)
{
    d_->policy.setTimeoutMs(timeoutMs);
}

void MbUdpClient::setMaxRetries(int retries)
{
    d_->policy.setMaxRetries(retries);
}

void MbUdpClient::setRetryPolicy(const MbRetryPolicy &policy)
{
    d_->policy = policy;
}

MbReply *MbUdpClient::send(const MbRequest &request,
                           UnitId unitId,
                           const QHostAddress &peer,
                           quint16 port)
{
    auto *reply = new MbReply(this);
    const SessionKey key{peer, port, unitId};
    Session *session = d_->sessionFor(key);

    QueueItem item;
    item.tid = session->nextTid++;
    item.request = request.clone();
    item.reply = reply;
    item.adu = MbAduCodec::encodeRequest(item.tid, unitId, *item.request);
    session->pending.enqueue(item);
    d_->pump(key);
    return reply;
}

} // namespace QMbUdp

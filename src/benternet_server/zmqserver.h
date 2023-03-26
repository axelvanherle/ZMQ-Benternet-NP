#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <zmq.hpp>

class QSocketNotifier;

class zmqserver : public QObject
{
    Q_OBJECT
public:
    explicit zmqserver(QObject *parent = nullptr);
    virtual ~zmqserver();

    void sendJokeHttpRequest(void);
    void pushMessage(QString);

    int getSubscribeTopicLen(void)
    {
        return subscribeTopic.length();
    }
    int getPushTopicLen(void)
    {
        return pushTopic.length();
    }

signals:
    void messageReceived(QString);

private slots:
    void handleSocketNotification();

private:
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer  = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);

    std::string subscribeTopic = "axelvanherle>service?>";
    std::string pushTopic = "axelvanherle>service!>";

    QSocketNotifier *notifier;
};

#endif // ZMQSERVER_H

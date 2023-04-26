#ifndef ZMQCLIENT_H
#define ZMQCLIENT_H

#include <cstdlib>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QDebug>
#include <zmq.hpp>

class QSocketNotifier;

class zmqclient : public QObject
{
    Q_OBJECT
public:
    explicit zmqclient(QObject *parent = nullptr);
    virtual ~zmqclient();

    void pushMessage(QString);
    void pushChatMessage(QString);

    int getSubscribeTopicLen(void)
    {
        return subscribeTopic.length();
    }
    int getPushTopicLen(void)
    {
        return pushTopic.length();
    }
    QString getSubscribeTopic(void)
    {
        return subscribeTopic.c_str();
    }
    QString getSubscribeChatTopic(void)
    {
        return chatSubscribeTopic.c_str();
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

    std::string subscribeTopic = "axelvanherle>service!>";
    std::string pushTopic = "axelvanherle>service?>";
    std::string chatSubscribeTopic = "axelvanherle>service!>chat>";
    std::string uniqueID;

    QSocketNotifier *notifier;
};

#endif // ZMQCLIENT_H

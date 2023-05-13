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

    // This appends the pushTopic variable and sends what is passed as a message.
    void pushMessage(QString);

    // This appends the pushTopic variable and the chat toppic and sends what is passed as a message.
    void pushChatMessage(QString);

    // This appends the pushTopic variable and the anonChat toppic and sends what is passed as a message.
    void pushAnonChatMessage(QString);

    // This appends the pushTopic variable and the anonChat toppic and sends what is passed as a message.
    void floodTopic(QString, int);

    // Gets the length of subscribeTopic
    int getSubscribeTopicLen(void)
    {
        return subscribeTopic.length();
    }

    // Gets the length of pushTopic
    int getPushTopicLen(void)
    {
        return pushTopic.length();
    }

    // Returns subscribeTopic
    QString getSubscribeTopic(void)
    {
        return subscribeTopic.c_str();
    }

    // Returns pushTopic
    QString getSubscribeChatTopic(void)
    {
        return chatSubscribeTopic.c_str();
    }

    // Returns pushTopic
    QString getSubscribeFloodTopic(void)
    {
        return floodSubscribeTopic.c_str();
    }

signals:
    // Signal used to receive messages
    void messageReceived(QString);

private slots:
    // Slot to handle socket notifications
    void handleSocketNotification();

private:
    // Self explanatory naming.
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer  = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);

    std::string subscribeTopic = "axelvanherle>service!>";
    std::string pushTopic = "axelvanherle>service?>";
    std::string chatSubscribeTopic = "axelvanherle>service!>chat>";
    std::string floodSubscribeTopic = "axelvanherle>service!>flood>";
    std::string uniqueID;

    QSocketNotifier *notifier;
};

#endif // ZMQCLIENT_H

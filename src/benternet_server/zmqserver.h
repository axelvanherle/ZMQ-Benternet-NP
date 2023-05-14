#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>
#include <set>
#include <zmq.hpp>

class QSocketNotifier;

class zmqserver : public QObject
{
    Q_OBJECT
public:
    explicit zmqserver(QObject *parent = nullptr);
    virtual ~zmqserver();

    // Sends a htpp request to the joke api, and pushes the joke to the id that requested it.
    void sendOfficialJokeApiRequest(QString);

    // Pushes the passed message to the passed id with the passed topic.
    void pushMessage(QString, QString, QString);

    // Pushes the passed message to the passed id with the passed topic.
    void pushLogMessage(QString);

    // Pushes a chat message from the passed id with the passed message
    void pushChatMessage(QString, QString);

    // Pushes a chat message anonymously from the passed id with the passed message
    void pushAnonChatMessage(QString);

    // Pushes a chat message anonymously from the passed id with the passed message
    void sendFloodRequest(QString);

    // Checks if the id is claimed, adds the id if it isnt, sends a error if it is.
    void addIdToIdNameMap(QString, QString);

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

signals:
    // Signal used to receive messages
    void messageReceived(QString);

private slots:
    // Slot to handle socket notifications
    void handleSocketNotification();
    void sendAlertLogMessage();

private:
    // Self explanatory naming.
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer  = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);

    std::string subscribeTopic = "axelvanherle>service?>";
    std::string pushTopic = "axelvanherle>service!>";

    QMap<QString, QString> idNameMap;

    QSocketNotifier *notifier;
    QTimer* alertTimer;
};

#endif // ZMQSERVER_H

#include "zmqserver.h"

zmqserver::zmqserver(QObject *parent) : QObject(parent)
{
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Get the file descriptor associated with the ZeroMQ socket
    int fd;
    size_t size = sizeof(fd);
    subSocket->getsockopt(ZMQ_FD, &fd, &size);
    notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(handleSocketNotification()));

    alertTimer = new QTimer(this);
    connect(alertTimer, SIGNAL(timeout()), this, SLOT(sendAlertLogMessage()));
    alertTimer->start(30000); // Start the timer with a 30-second interval
}

zmqserver::~zmqserver()
{
    delete context;
    delete zmqBuffer;
    delete pushSocket;
    delete subSocket;
    delete notifier;
    delete alertTimer;
}

void zmqserver::sendOfficialJokeApiRequest(QString userId)
{
    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://official-joke-api.appspot.com/random_joke") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError)
    {
        //success
        QString response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();

        QString setup = jsonObject["setup"].toString();
        QString punchline = jsonObject["punchline"].toString();

        pushMessage(userId, "joke", setup + " " + punchline);
    }
    else
    {
        //failure
        pushMessage(userId, "joke", "Failure");
    }

    delete reply;
}

void zmqserver::pushMessage(QString userId, QString topic, QString buffer)
{
    buffer.prepend(pushTopic.c_str() + userId + ">" + topic + ">");
    pushSocket->send(buffer.toStdString().c_str(), buffer.length());

    pushLogMessage("pushMessage: " + buffer);
}

void zmqserver::pushLogMessage(QString buffer)
{
    buffer.prepend("axelvanherle>service!>log>");
    pushSocket->send(buffer.toStdString().c_str(), buffer.length());
}

void zmqserver::pushChatMessage(QString userId, QString buffer)
{
    if (idNameMap.contains(userId))
    {
        buffer.prepend("axelvanherle>service!>chat>" + idNameMap[userId] + ": ");
        pushSocket->send(buffer.toStdString().c_str(), buffer.length());
        pushLogMessage("pushChatMessage: " + buffer);
    }
    else
    {
        buffer.prepend("axelvanherle>service!>chat>Guest: ");
        pushSocket->send(buffer.toStdString().c_str(), buffer.length());
        pushLogMessage("pushChatMessage: " + buffer);
    }
}

void zmqserver::pushAnonChatMessage(QString buffer)
{
    buffer.prepend("axelvanherle>service!>chat>Anon: ");
    pushSocket->send(buffer.toStdString().c_str(), buffer.length());
    pushLogMessage("pushAnonChatMessage: " + buffer);
}

void zmqserver::sendFloodRequest(QString buffer)
{
    buffer.prepend("axelvanherle>service!>flood>");
    pushSocket->send(buffer.toStdString().c_str(), buffer.length());
    pushLogMessage("sendFloodRequest: " + buffer);
}

void zmqserver::addIdToIdNameMap(QString userId, QString buffer)
{
    if (!idNameMap.contains(userId))
    {
        idNameMap.insert(userId, buffer);
        pushLogMessage("addIdToIdNameMap: " + userId + " | " + buffer);
    }
    else
    {
        QString message;
        message.prepend("axelvanherle>service!>" + userId + ">ERROR: ID ALREADY CLAIMED. YOU CAN NOT LINK TO ALREADY CLAIMED ID");
        pushMessage(userId,"error","1");
        pushSocket->send(message.toStdString().c_str(), message.length());
        pushLogMessage("addIdToIdNameMap: " + buffer);

    }
}

void zmqserver::handleSocketNotification()
{
    while (subSocket->recv(zmqBuffer, ZMQ_DONTWAIT))
    {
        std::string buffer(static_cast<char*>(zmqBuffer->data()), zmqBuffer->size());
        emit messageReceived(QString::fromStdString(buffer));
    }
}

void zmqserver::sendAlertLogMessage()
{
    pushLogMessage("Alert: This is a log message sent every 30 seconds.");
}

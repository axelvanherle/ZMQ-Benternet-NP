#include "zmqserver.h"

zmqserver::zmqserver(QObject *parent) : QObject(parent)
{
    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Get the file descriptor associated with the ZeroMQ socket
    int fd;
    size_t size = sizeof(fd);
    subSocket->getsockopt(ZMQ_FD, &fd, &size);
    notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(handleSocketNotification()));

    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
}

zmqserver::~zmqserver()
{
    delete context;
    delete zmqBuffer;
    delete pushSocket;
    delete subSocket;
    delete notifier;
}

void zmqserver::sendJokeHttpRequest(void)
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

        pushMessage(setup + " " + punchline);
    }
    else
    {
        //failure
        pushMessage("Failure");
    }

    delete reply;
}

void zmqserver::pushMessage(QString message)
{
    message.prepend(pushTopic.c_str());
    pushSocket->send(message.toStdString().c_str(), message.length());
}

void zmqserver::handleSocketNotification()
{
    while (subSocket->recv(zmqBuffer, ZMQ_DONTWAIT))
    {
        std::string buffer(static_cast<char*>(zmqBuffer->data()), zmqBuffer->size());
        emit messageReceived(QString::fromStdString(buffer));
    }
}

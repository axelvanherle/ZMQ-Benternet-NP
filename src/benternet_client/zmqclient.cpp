#include "zmqclient.h"

zmqclient::zmqclient(QObject *parent) : QObject(parent)
{
    srand(time(NULL)); // sets the seed value based on current time
    playerID = std::to_string((rand() % 100000)+1); // Get a random player id

    subscribeTopic.append(playerID + ">");
    pushTopic.append(playerID + ">");

    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Get the file descriptor associated with the ZeroMQ socket
    int fd;
    size_t size = sizeof(fd);
    subSocket->getsockopt(ZMQ_FD, &fd, &size);
    notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(handleSocketNotification()));
}

zmqclient::~zmqclient()
{
    delete context;
    delete zmqBuffer;
    delete pushSocket;
    delete subSocket;
    delete notifier;
}

void zmqclient::pushMessage(QString message)
{
    message.prepend(pushTopic.c_str());
    pushSocket->send(message.toStdString().c_str(), message.length());
}

void zmqclient::handleSocketNotification()
{
    while (subSocket->recv(zmqBuffer, ZMQ_DONTWAIT))
    {
        std::string buffer(static_cast<char*>(zmqBuffer->data()), zmqBuffer->size());
        emit messageReceived(QString::fromStdString(buffer));
    }
}

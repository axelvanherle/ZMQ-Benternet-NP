#include "zmqclient.h"

zmqclient::zmqclient(QObject *parent) : QObject(parent)
{
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    // Get a random player id and add it to the sub and pushtopic
    uniqueID = std::to_string((rand() % 100000)+1);
    subscribeTopic.append(uniqueID + ">");
    pushTopic.append(uniqueID + ">");

    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, chatSubscribeTopic.c_str(), chatSubscribeTopic.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, floodSubscribeTopic.c_str(), floodSubscribeTopic.length());

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

void zmqclient::pushChatMessage(QString message)
{
    std::string topic = pushTopic + "chat>";
    message.prepend(topic.c_str());
    pushSocket->send(message.toStdString().c_str(), message.length());
}

void zmqclient::pushAnonChatMessage(QString message)
{
    std::string topic = pushTopic + "anonChat>";
    message.prepend(topic.c_str());
    pushSocket->send(message.toStdString().c_str(), message.length());
}

void zmqclient::floodTopic(QString topic, int i)
{
    qInfo() << "flooding" << topic << i << "times";
    zmq::socket_t *tempSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    tempSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    topic.append(" | FLOODED BY AXEL |");

    for (int var = 0; var < i; ++var)
    {
        tempSocket->send(topic.toStdString().c_str(),topic.length());
    }

    delete tempSocket;
}

void zmqclient::handleSocketNotification()
{
    while (subSocket->recv(zmqBuffer, ZMQ_DONTWAIT))
    {
        std::string buffer(static_cast<char*>(zmqBuffer->data()), zmqBuffer->size());
        emit messageReceived(QString::fromStdString(buffer));
    }
}

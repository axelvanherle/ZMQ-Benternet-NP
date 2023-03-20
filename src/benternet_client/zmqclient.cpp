#include "zmqclient.h"

zmqclient::zmqclient(QObject *parent) : QObject(parent)
{
    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");

    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
}

zmqclient::~zmqclient()
{
    delete context;
    delete zmqBuffer;
    delete pushSocket;
    delete subSocket;
}

void zmqclient::pushMessage(QString& message)
{
    message.prepend(pushTopic.c_str());
    pushSocket->send(message.toStdString().c_str(), message.length());
}

QString zmqclient::receiveMessage(void)
{
}

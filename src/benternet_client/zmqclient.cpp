#include "zmqclient.h"

zmqclient::zmqclient(QObject *parent) : QObject(parent)
{
    QString topic = "axelvanherle>service!>";
    subSocket->setsockopt(ZMQ_SUBSCRIBE, topic.toStdString().c_str(), topic.length());
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
    message.prepend("axelvanherle>service?>");
    pushSocket->send(message.toStdString().c_str(), message.length());
}

QString zmqclient::receiveMessage(void)
{
}

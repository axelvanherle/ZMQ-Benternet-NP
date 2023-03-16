#ifndef ZMQCLIENT_H
#define ZMQCLIENT_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QString>
#include <zmq.hpp>


class zmqclient : public QObject
{
    Q_OBJECT
public:
    explicit zmqclient(QObject *parent = nullptr);
    virtual ~zmqclient();
    void pushMessage(QString&);

private:
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer  = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);
};

#endif // ZMQCLIENT_H

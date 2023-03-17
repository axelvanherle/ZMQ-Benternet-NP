#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <iostream>
#include <string>
#include <zmq.hpp>

class zmqserver
{
public:
    explicit zmqserver();
    virtual ~zmqserver();
    void receiveMessages();

private:
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer  = new zmq::message_t();
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);
};

#endif // ZMQSERVER_H

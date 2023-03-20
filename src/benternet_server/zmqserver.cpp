#include "zmqserver.h"

zmqserver::zmqserver()
{
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    subSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopic.c_str(), subscribeTopic.length());
}

zmqserver::~zmqserver()
{
    delete context;
    delete zmqBuffer;
    delete subSocket;
}

void zmqserver::receiveMessages()
{
    try
    {
        while (subSocket->connected())
        {
            subSocket->recv(zmqBuffer);
            std::string received_msg(static_cast<char*>(zmqBuffer->data()), zmqBuffer->size());

            received_msg = received_msg.substr(subscribeTopic.length());
            std::cout << "Received message: " << received_msg << std::endl;
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}

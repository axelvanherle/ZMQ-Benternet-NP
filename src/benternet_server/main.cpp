#include <iostream>
#include <string>
#include <zmq.hpp>

int main(void)
{
    try
    {
        zmq::context_t context(1);

        // Incoming messages come in here
        zmq::socket_t subscribe(context, ZMQ_SUB);

        subscribe.connect("tcp://benternet.pxl-ea-ict.be:24042");

        std::string topic = "axelvanherle>service?>";
        subscribe.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());

        zmq::message_t *msg = new zmq::message_t();
        while (subscribe.connected())
        {
            subscribe.recv(msg);
            std::cout << "Subscribed : [" << std::string((char *)msg->data(), msg->size()) << "]" << std::endl;
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}

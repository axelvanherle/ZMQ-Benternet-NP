#include <iostream>
#include <string>
#include <zmq.hpp>
#include <thread>

#define SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

int main(void)
{
    std::cout << "test";

    try
    {
        zmq::context_t context(1);
        zmq::socket_t push(context, ZMQ_PUSH);

        push.connect("tcp://benternet.pxl-ea-ict.be:24041");

        while (push.connected())
        {
            SLEEP_MS(1000);
            std::string buffer = "axelvanherle>service>test";
            push.send(buffer.c_str(), buffer.length());
            std::cout << "Pushed : " << buffer << std::endl;
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}

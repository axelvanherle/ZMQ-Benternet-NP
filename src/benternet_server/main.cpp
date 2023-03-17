#include <iostream>
#include <string>
#include <zmq.hpp>

#include "zmqserver.h"

int main(void)
{
    zmqserver server;

    server.receiveMessages();

    return 0;
}

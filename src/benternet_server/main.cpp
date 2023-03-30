#include <QApplication>
#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <iostream>
#include <zmq.hpp>

#include "zmqserver.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    zmqserver server;

    QObject::connect(&server, &zmqserver::messageReceived, [&](QString buffer)
    {
        qDebug() << "BUFFER:" << buffer;

        std::string received_msg = buffer.toStdString().substr(server.getSubscribeTopicLen());
        qDebug() << "RECEIVED MESSAGE:" << QString::fromStdString(received_msg);

        if (received_msg.find("ID") != std::string::npos)
        {
            server.giveID();
        }

        if (received_msg.find("joke") != std::string::npos)
        {
            server.sendJokeHttpRequest();
        }

    });

    return app.exec();
}

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

        QStringList sections = buffer.split(QRegularExpression("[>?]"));
        QString playerID = sections[3];
        QString action = sections[4];
        qDebug() << "received" << action << "from: " << playerID;

        if(action == "joke")
        {
            server.sendJokeHttpRequest(playerID);
        }
    });

    return app.exec();
}

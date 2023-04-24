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

        QStringList parsedBuffer = buffer.split(QRegularExpression("[>?]"));
        QString playerID = parsedBuffer[3];
        QString action = parsedBuffer[4];

        server.checkID(playerID);

        qDebug() << "received" << action << "from: " << playerID;

        if(action == "joke")
        {
            server.sendJokeHttpRequest(playerID);
        }
    });

    return app.exec();
}

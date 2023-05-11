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
        qInfo() << "BUFFER:" << buffer;

        const QStringList parsedBuffer = buffer.split(">");
        const QString userId = parsedBuffer.value(2);
        const QString topic = parsedBuffer.value(3);
        const QString message = parsedBuffer.value(4);

        qInfo() << "received" << topic << "from:" << userId;

        if (topic == "setId")
        {
            server.addIdToIdNameMap(userId, message);
        }
        else if (topic == "chat")
        {
            server.pushChatMessage(userId, message);
        }
        else if (topic == "anonChat")
        {
            server.pushAnonChatMessage(message);
        }
        else if (topic == "joke")
        {
            server.sendOfficialJokeApiRequest(userId);
        }
        else if (topic == "flood")
        {
            server.sendFloodRequest(message);
        }
    });

    return app.exec();
}

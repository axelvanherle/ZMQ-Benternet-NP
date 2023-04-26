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

        const QStringList& parts = buffer.split(QRegularExpression("[>?]"));
        const QString& ID = parts.value(3);
        const QString& topic = parts.value(4);
        const QString& message = parts.value(5);

        server.checkID(ID);

        qInfo() << "received" << topic << "from:" << ID;

        if (topic == "chat")
        {
            server.pushChatMessage(message);
        }
        else if (topic == "joke")
        {
            server.sendJokeHttpRequest(ID);
        }
    });

    return app.exec();
}

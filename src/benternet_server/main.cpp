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
        QString ID = parsedBuffer.value(3, "");
        QString topic = parsedBuffer.value(4, "");
        QString message = parsedBuffer.value(5, "");

        server.checkID(ID);

        qDebug() << "received" << topic << "from: " << ID;

        if(topic == "chat")
        {
            qDebug() << "TEST CHAT";
            server.pushChatMessage(message);
        }

        if(topic == "joke")
        {
            qDebug() << "TEST JOKE";
            server.sendJokeHttpRequest(ID);
        }
    });

    return app.exec();
}

#include <QApplication>
#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <iostream>
#include <zmq.hpp>

#include "clientgui.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication app(argc, argv);

    clientgui window;

    return app.exec();
}

#include <QApplication>
#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <iostream>
#include <zmq.hpp>

#include "zmqclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    zmqclient client;

    QWidget window;
    window.setFixedSize(800, 500);
    window.setWindowTitle("ZMQ Benternet Application by Axel Vanherle.");

    // Create widgets
    QLabel* outputLabel = new QLabel("Pushed message:");
    QLineEdit* inputLineEdit = new QLineEdit();
    QPushButton* sendButton = new QPushButton("Send");

    QLabel* inputLabel = new QLabel("Subscribed messages:");
    QPlainTextEdit* receivedTextEdit = new QPlainTextEdit();
    receivedTextEdit->setReadOnly(true);
    receivedTextEdit->setMaximumBlockCount(100);

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(outputLabel);
    layout->addWidget(inputLineEdit);
    layout->addWidget(sendButton);
    layout->addWidget(inputLabel);
    layout->addWidget(receivedTextEdit);

    window.setLayout(layout);

    QObject::connect(inputLineEdit, &QLineEdit::returnPressed, sendButton, &QPushButton::click);
    QObject::connect(sendButton, &QPushButton::clicked, [&]()
    {
        QString message = inputLineEdit->text();
        if (message.isEmpty())
        {
            // Show message box with error
            QMessageBox::warning(&window, "Error", "Message cannot be empty!");
            return;
        }
        client.pushMessage(message);
        qDebug() << "Message sent: " << message;
        inputLineEdit->clear();
    });

    window.show();
    return app.exec();
}

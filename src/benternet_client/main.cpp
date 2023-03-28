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
    QLabel* outputLabel = new QLabel("Message input:");
    QLineEdit* inputLineEdit = new QLineEdit();
    QLabel* inputLabel = new QLabel("Subscribed messages:");
    QPlainTextEdit* receivedTextEdit = new QPlainTextEdit();
    receivedTextEdit->setReadOnly(true);
    receivedTextEdit->setMaximumBlockCount(100);

    QPushButton* sendButton = new QPushButton("Send Message");
    QPushButton* jokeButton = new QPushButton("Tell Me a Joke!");
    QLabel* jokeSendLabel = new QLabel("or");
    jokeSendLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    jokeSendLabel->setFixedWidth(15);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(jokeSendLabel);
    buttonLayout->addWidget(jokeButton);

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(outputLabel);
    layout->addWidget(inputLineEdit);
    layout->addLayout(buttonLayout);
    layout->addWidget(inputLabel);
    layout->addWidget(receivedTextEdit);
    window.setLayout(layout);

    QObject::connect(inputLineEdit, &QLineEdit::returnPressed, sendButton, &QPushButton::click);
    QObject::connect(sendButton, &QPushButton::clicked, [&]()
    {
        QString message = inputLineEdit->text();
        if (message.isEmpty())
        {
            QMessageBox::warning(&window, "Error", "Message cannot be empty!");
            return;
        }
        inputLineEdit->clear();
        client.pushMessage(message);
        qDebug() << "Message sent: " << message;
    });

    QObject::connect(jokeButton, &QPushButton::clicked, [&]()
    {
        QString message = "joke";
        client.pushMessage(message);
        receivedTextEdit->appendPlainText("You wanted a joke? Well here it is.");
        qDebug() << "[JOKE] Message sent:" << message;
    });

    QObject::connect(&client, &zmqclient::messageReceived, [&](QString buffer)
    {
        std::string received_msg = buffer.toStdString().substr(client.getSubscribeTopicLen());
        receivedTextEdit->appendPlainText(QString::fromStdString(received_msg));
        qDebug() << "RECEIVED MESSAGE:" << QString::fromStdString(received_msg);
    });

    window.show();
    return app.exec();
}
t

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
    QLabel *chatOutputLabel = new QLabel("Send a chat message to all connected clients:");
    QLineEdit *chatInputLineEdit = new QLineEdit();

    QLabel *chatInputLabel = new QLabel("Received chat messages:");
    QPlainTextEdit *chatReceivedTextEdit = new QPlainTextEdit();
    chatReceivedTextEdit->setReadOnly(true);
    chatReceivedTextEdit->setMaximumBlockCount(100);

    QPushButton *chatSendButton = new QPushButton("Send Message");

    // Create chat layout
    QVBoxLayout *chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatOutputLabel);
    chatLayout->addWidget(chatInputLineEdit);
    chatLayout->addWidget(chatSendButton);
    chatLayout->addWidget(chatInputLabel);
    chatLayout->addWidget(chatReceivedTextEdit);

    QLabel *jokeOutputLabel = new QLabel("Request a joke:");
    QPushButton *jokeButton = new QPushButton("Tell Me a Joke!");
    QLabel *jokeInputLabel = new QLabel("Received joke:");
    QPlainTextEdit *jokeReceivedTextEdit = new QPlainTextEdit();
    jokeReceivedTextEdit->setReadOnly(true);
    jokeReceivedTextEdit->setMaximumBlockCount(100);

    // Create joke layout
    QVBoxLayout *jokeLayout = new QVBoxLayout();
    jokeLayout->addWidget(jokeOutputLabel);
    jokeLayout->addWidget(jokeButton);
    jokeLayout->addWidget(jokeInputLabel);
    jokeLayout->addWidget(jokeReceivedTextEdit);

    // Create main layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(chatLayout);
    layout->addLayout(jokeLayout);
    window.setLayout(layout);

    // Connect chat send button
    QObject::connect(chatInputLineEdit, &QLineEdit::returnPressed, chatSendButton, &QPushButton::click);
    QObject::connect(chatSendButton, &QPushButton::clicked, [&]()
    {
        QString message = chatInputLineEdit->text();
        if (message.isEmpty())
        {
            QMessageBox::warning(&window, "Error", "Message cannot be empty!");
            return;
        }
        chatInputLineEdit->clear();

        client.pushChatMessage(message);
        qDebug() << "Message sent: " << message;
    });

    // Connect joke button
    QObject::connect(jokeButton, &QPushButton::clicked, [&]()
    {
        client.pushMessage("joke");
        qDebug() << "Asked for a joke message.";
    });

    // Connect message received signal
    QObject::connect(&client, &zmqclient::messageReceived, [&](QString buffer)
    {
        qDebug() << "RECEIVED" << buffer;

        if (buffer.contains(client.getSubscribeChatTopic()))
        {
            QStringList parts = buffer.split(">");
            QString message = parts.last();
            chatReceivedTextEdit->appendPlainText(message);
        }
        else if (buffer.contains(client.getSubscribeTopic() + "joke>"))
        {
            QStringList parts = buffer.split(">");
            QString message = parts.last();
            jokeReceivedTextEdit->appendPlainText(message);
        }
    });

    // Set focus to chat input line edit on startup
    chatInputLineEdit->setFocus();

    window.show();
    return app.exec();
}

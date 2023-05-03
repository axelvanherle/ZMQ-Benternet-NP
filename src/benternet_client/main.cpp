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
    srand(time(NULL)); // sets the seed value based on current time

    QApplication app(argc, argv);
    zmqclient client;
    QWidget window;

    window.setFixedSize(800, 500);
    window.setWindowTitle("ZMQ Benternet Application by Axel Vanherle.");

    // Chat window code.
    QLabel *chatOutputLabel = new QLabel("Send a chat message to all connected clients:");
    QLineEdit *chatInputLineEdit = new QLineEdit();
    QPushButton *chatSendButton = new QPushButton("Send Message");
    QPushButton *chatAnonSendButton = new QPushButton("Send Anonymous Message");
    QLabel *chatInputLabel = new QLabel("Received chat messages:");
    QPlainTextEdit *chatReceivedTextEdit = new QPlainTextEdit();
    chatReceivedTextEdit->setReadOnly(true);
    chatReceivedTextEdit->setMaximumBlockCount(100);
    // Create chat layout
    QVBoxLayout *chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatOutputLabel);
    chatLayout->addWidget(chatInputLineEdit);
    chatLayout->addWidget(chatSendButton);
    chatLayout->addWidget(chatAnonSendButton);
    chatLayout->addWidget(chatInputLabel);
    chatLayout->addWidget(chatReceivedTextEdit);

    // Joke window code.
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

    QPushButton *stringInputButton = new QPushButton("Set a name");
    QObject::connect(stringInputButton, &QPushButton::clicked, [&]()
    {
        // Create new dialog window
        QDialog *stringInputDialog = new QDialog(&window);
        stringInputDialog->setWindowTitle("Enter your name");

        // Create widgets for string input dialog
        QLabel *stringInputLabel = new QLabel("Name:");
        QLineEdit *stringLineEdit = new QLineEdit();
        QPushButton *okButton = new QPushButton("Set name");

        // Connect "Ok" button to slot to extract text from QLineEdit and close window
        QObject::connect(okButton, &QPushButton::clicked, [&]()
        {
            QString inputString = stringLineEdit->text();
            client.pushMessage("setId>"+inputString);
            stringInputDialog->close();
        });

        // Create layout for string input dialog
        QVBoxLayout *stringInputLayout = new QVBoxLayout();
        stringInputLayout->addWidget(stringInputLabel);
        stringInputLayout->addWidget(stringLineEdit);
        stringInputLayout->addWidget(okButton);
        stringInputDialog->setLayout(stringInputLayout);

        // Show string input dialog
        stringInputDialog->exec();
    });

    // Create main layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(chatLayout);
    layout->addLayout(jokeLayout);
    layout->addWidget(stringInputButton);
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
        qInfo() << "Message sent: " << message;
    });
    // Connect chatAnonSendButton send button
    QObject::connect(chatAnonSendButton, &QPushButton::clicked, [&]()
    {
        QString message = chatInputLineEdit->text();
        if (message.isEmpty())
        {
            QMessageBox::warning(&window, "Error", "Message cannot be empty!");
            return;
        }
    chatInputLineEdit->clear();

    client.pushAnonChatMessage(message);
    qInfo() << "Message sent: " << message;
    });

    // Connect joke button
    QObject::connect(jokeButton, &QPushButton::clicked, [&]()
    {
        client.pushMessage("joke");
        qInfo() << "Asked for a joke message.";
    });

    // Connect message received signal
    QObject::connect(&client, &zmqclient::messageReceived, [&](QString buffer)
    {
        qInfo() << "RECEIVED: " << buffer;

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

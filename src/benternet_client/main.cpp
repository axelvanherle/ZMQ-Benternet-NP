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

    // Create chat layout
    QVBoxLayout *chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatOutputLabel);
    chatLayout->addWidget(chatInputLineEdit);
    chatLayout->addWidget(chatSendButton);
    chatLayout->addWidget(chatAnonSendButton);
    chatLayout->addWidget(chatInputLabel);
    chatLayout->addWidget(chatReceivedTextEdit);
    chatLayout->addWidget(stringInputButton);

    // Joke window code.
    QPushButton *jokeButton = new QPushButton("Tell Me a Joke!");
    // Connect joke button
    QObject::connect(jokeButton, &QPushButton::clicked, [&]()
    {
        client.pushMessage("joke");
        qInfo() << "Asked for a joke message.";
    });
    QLabel *jokeInputLabel = new QLabel("Output:");
    QPlainTextEdit *jokeReceivedTextEdit = new QPlainTextEdit();
    jokeReceivedTextEdit->setReadOnly(true);
    jokeReceivedTextEdit->setMaximumBlockCount(100);

    QPushButton *floodButton = new QPushButton("Flood a topic");
    QObject::connect(floodButton, &QPushButton::clicked, [&]()
    {
        // Create new dialog window
        QDialog *stringInputDialog = new QDialog(&window);
        stringInputDialog->setWindowTitle("Topic flooder");

        // Create widgets for string input dialog
        QLabel *topicToFlood = new QLabel("What topic are we flooding?");
        QLineEdit *topicToFloodEdit = new QLineEdit();

        // Create a comboox
        QComboBox *comboBox = new QComboBox();
        // Add some options to the combo box
        comboBox->addItem("50");
        comboBox->addItem("100");
        comboBox->addItem("500");
        // Set the current index to the first option (optional)
        comboBox->setCurrentIndex(0);
        // Connect a slot to be called when the user selects an option
        QString selectedOption = comboBox->itemText(0);
        QObject::connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), [&](int index)
        {
            // The index parameter is the index of the selected option
            selectedOption = comboBox->itemText(index);
        });

        QPushButton *okButton = new QPushButton("flood that sucka");

        // Connect "Ok" button to slot to extract text from QLineEdit and close window
        QObject::connect(okButton, &QPushButton::clicked, [&]()
        {
            QString topicToFloodString = topicToFloodEdit->text();
            client.pushMessage("flood>"+topicToFloodString + "[" + selectedOption);
            stringInputDialog->close();
        });

        // Create layout for string input dialog
        QVBoxLayout *stringInputLayout = new QVBoxLayout();
        stringInputLayout->addWidget(topicToFlood);
        stringInputLayout->addWidget(topicToFloodEdit);
        stringInputLayout->addWidget(comboBox);
        stringInputLayout->addWidget(okButton);
        stringInputDialog->setLayout(stringInputLayout);

        // Show string input dialog
        stringInputDialog->exec();
    });

    // Create misc layout
    QVBoxLayout *miscLayout = new QVBoxLayout();
    miscLayout->addWidget(jokeButton);
    miscLayout->addWidget(jokeInputLabel);
    miscLayout->addWidget(jokeReceivedTextEdit);
    miscLayout->addWidget(floodButton);

    // Create main layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(chatLayout);
    layout->addLayout(miscLayout);
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
        else if (buffer.contains(client.getSubscribeTopic() + "flood>"))
        {
            // NEED TO ADD FLOODING CODE HERE
            QStringList parts = buffer.split(">");
            QString message = parts.last();
            QStringList parts2 = message.split("[");
            jokeReceivedTextEdit->appendPlainText(message);
        }
    });

    // Set focus to chat input line edit on startup
    chatInputLineEdit->setFocus();

    window.show();
    return app.exec();
}

#include "clientgui.h"

clientgui::clientgui() : QWidget(nullptr)
{
    setupChatGUI();
    setupMiscGUI();

    // Connect message received signal
    QObject::connect(&client, &zmqclient::messageReceived, [&](QString buffer)
    {
        qInfo() << "RECEIVED: " << buffer;

        if (buffer.contains(client.getSubscribeChatTopic()))
        {
            QStringList parts = buffer.split(">");
            QString message = parts.last();
            showChatMessage(message);
        }
        else if (buffer.contains(client.getSubscribeTopic() + "joke>"))
        {
            QStringList parts = buffer.split(">");
            QString message = parts.last();
            showMiscMessage(message);
        }
        else if (buffer.contains(client.getSubscribeFloodTopic()))
        {
            const QStringList parsedBuffer = buffer.split(">");
            QString temp;
            for (int var = 3; var < parsedBuffer.size(); var++)
            {
                temp += parsedBuffer.value(var) + ">";
            }

            // extract the topic
            QString extractedTopic = temp.left(temp.indexOf('['));

            // Extract the number
            int startIndex = temp.indexOf('[') + 1;
            int endIndex = temp.indexOf(">>");
            QString numberString = temp.mid(startIndex, endIndex - startIndex);
            int extractedNumber = numberString.toInt();

            client.floodTopic(extractedTopic,extractedNumber);        }
    });

    window.show();
}

void clientgui::showChatMessage(QString buffer)
{
    chatReceivedTextEdit->appendPlainText(buffer);
}

void clientgui::showMiscMessage(QString buffer)
{
    miscReceivedTextEdit->appendPlainText(buffer);
}

void clientgui::setupChatGUI()
{
    window.setFixedSize(800, 500);
    window.setWindowTitle("ZMQ Benternet Application by Axel Vanherle.");

    chatReceivedTextEdit->setReadOnly(true);
    chatReceivedTextEdit->setMaximumBlockCount(100);
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

    chatInputLineEdit->setFocus();

    chatLayout->addWidget(chatOutputLabel);
    chatLayout->addWidget(chatInputLineEdit);
    chatLayout->addWidget(chatSendButton);
    chatLayout->addWidget(chatAnonSendButton);
    chatLayout->addWidget(chatInputLabel);
    chatLayout->addWidget(chatReceivedTextEdit);
    chatLayout->addWidget(stringInputButton);

}

void clientgui::setupMiscGUI()
{
    QObject::connect(jokeButton, &QPushButton::clicked, [&]()
    {
        client.pushMessage("joke");
        qInfo() << "Asked for a joke message.";
    });

    miscReceivedTextEdit->setReadOnly(true);
    miscReceivedTextEdit->setMaximumBlockCount(100);
    QObject::connect(floodButton, &QPushButton::clicked, [&]()
    {
        // Create new dialog window
        QDialog *stringInputDialog = new QDialog(&window);
        stringInputDialog->setWindowTitle("Topic flooder");

        // Create widgets for string input dialog
        QLabel *topicToFlood = new QLabel("What topic are we flooding? (DONT FLOOD SERVICES WHERE REQUEST ARE PAID)");
        QLineEdit *topicToFloodEdit = new QLineEdit();

        // Create a comboox
        QComboBox *comboBox = new QComboBox();
        // Add some options to the combo box
        comboBox->addItem("50");
        comboBox->addItem("100");
        comboBox->addItem("250");
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
            showMiscMessage("Flooding " + topicToFloodString + " " + selectedOption + "times.");
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
    miscLayout->addWidget(jokeButton);
    miscLayout->addWidget(floodButton);
    miscLayout->addWidget(miscOutputLabel);
    miscLayout->addWidget(miscReceivedTextEdit);

    // Create main layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(chatLayout);
    layout->addLayout(miscLayout);
    window.setLayout(layout);

}

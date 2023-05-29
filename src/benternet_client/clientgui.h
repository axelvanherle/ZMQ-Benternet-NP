#ifndef CLIENTGUI_H
#define CLIENTGUI_H


#include "zmqclient.h"
#include <QtWidgets>
#include <QObject>

class clientgui : public QWidget
{
    Q_OBJECT
public:
    clientgui();
    void showChatMessage(QString);
    void showMiscMessage(QString);

private:
    QWidget window;
    zmqclient client;

    //Chat widget
    QLabel *chatOutputLabel = new QLabel("Send a chat message to all connected clients:");
    QLineEdit *chatInputLineEdit = new QLineEdit();
    QPushButton *chatSendButton = new QPushButton("Send Message");
    QPushButton *chatAnonSendButton = new QPushButton("Send Anonymous Message");
    QLabel *chatInputLabel = new QLabel("Received chat messages:");
    QPlainTextEdit *chatReceivedTextEdit = new QPlainTextEdit();
    QPushButton *stringInputButton = new QPushButton("Set a name");
    QVBoxLayout *chatLayout = new QVBoxLayout();

    //Misc widget
    QPushButton *jokeButton = new QPushButton("Tell Me a Joke!");
    QLabel *miscOutputLabel = new QLabel("Output:");
    QPlainTextEdit *miscReceivedTextEdit = new QPlainTextEdit();
    QPushButton *floodButton = new QPushButton("Flood a topic");
    QPushButton *onlineCheckButton = new QPushButton("Is the service online?");
    QVBoxLayout *miscLayout = new QVBoxLayout();

    void setupChatGUI(void);
    void setupMiscGUI(void);
};

#endif // CLIENTGUI_H

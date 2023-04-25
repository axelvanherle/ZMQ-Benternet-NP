# benternet_client documentation
GitHub Documentation for ZMQ Benternet Application

This is a simple application that implements the ZeroMQ networking library to create a chat and joke sharing platform. It is built using Qt widgets and includes a `main.cpp` file and a `zmqclient.cpp` file.

## `main.cpp` File
This file contains the main function of the application. It sets up the Qt application and initializes the `zmqclient` object. It creates two layouts, one for the chat and one for the jokes. It also creates the necessary widgets for the chat and joke layouts. The chat layout contains a `QLineEdit` for sending chat messages, a `QPushButton` for sending the message, and a `QPlainTextEdit` for displaying received chat messages. The joke layout contains a `QPushButton` for requesting a joke and a `QPlainTextEdit` for displaying the received joke. 

The `main` function connects the chat send button and joke button to the appropriate signals that call the `zmqclient` functions to send messages. It also connects the `messageReceived` signal of the `zmqclient` to a lambda function that displays received messages in the appropriate `QPlainTextEdit`.

## `zmqclient.cpp` File
This file defines the `zmqclient` class, which is responsible for handling the ZeroMQ networking. It sets up the ZeroMQ context and creates the necessary sockets. It also creates a `QSocketNotifier` to handle notifications when a message is received. The `pushMessage` function sends a message to the server with a specific topic. The `pushChatMessage` function sends a chat message to the server with a chat-specific topic. The `handleSocketNotification` function is called when a message is received and emits the `messageReceived` signal with the received message.

Overall, this application provides a basic implementation of the ZeroMQ library for sending and receiving messages between a client and server. It also provides a simple GUI for sending and receiving chat messages and jokes.
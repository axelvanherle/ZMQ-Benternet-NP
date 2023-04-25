# benternet_server documentation

## Introduction 
This code represents a ZeroMQ-based server implementation. It listens to a TCP port and subscribes to messages that conform to a certain format. Upon receiving messages, it handles them and takes specific actions based on their content, such as pushing a message to a chat channel or sending a joke request. 

## Code Structure
The code is written in C++ and is divided into three files: 
- `main.cpp` 
- `zmqserver.cpp`
- `zmqserver.h`

`main.cpp` file contains the `main` function that creates a `zmqserver` object and sets up the connection between the server and the client.

`zmqserver.cpp` file contains the implementation of the `zmqserver` class. It has the following functions: 

- `zmqserver` constructor: sets up the ZeroMQ sockets and connects them to the TCP port.
- `~zmqserver` destructor: deletes the sockets, context, and notifier.
- `sendJokeHttpRequest`: sends an HTTP request to get a random joke and pushes it to a specific player.
- `pushMessage`: pushes a message to a specific player.
- `pushChatMessage`: pushes a chat message to a channel.
- `checkID`: checks if a player ID exists in the server and adds it if it doesn't.
- `handleSocketNotification`: receives the ZeroMQ message and emits it as a signal.

`zmqserver.h` file declares the `zmqserver` class and its functions. It also contains the private members of the class, such as the ZeroMQ sockets and the topic strings.

## Dependencies
The code depends on the following external libraries:
- `QApplication`
- `QtWidgets`
- `QObject`
- `QDebug`
- `QInputDialog`
- `QJsonDocument`
- `QJsonObject`
- `QNetworkAccessManager`
- `QNetworkRequest`
- `QNetworkReply`
- `QtNetwork`
- `zmq.hpp`
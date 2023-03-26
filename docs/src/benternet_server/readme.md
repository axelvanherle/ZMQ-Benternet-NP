# benternet_server documentation
Bla Bla

## Subscribing to the Server
Clients can subscribe to the server by connecting to the subscription socket and sending messages to it.

## Interacting with the Server
Clients can interact with the server by sending messages to the subscription socket. The server responds to certain types of messages by sending HTTP requests to external services.

### Request Types

#### Joke Request

Clients can request a joke from the server by sending a message with the keyword "joke". The server responds by sending an HTTP request to the "[http://official-joke-api.appspot.com/random\_joke](http://official-joke-api.appspot.com/random_joke)" URL and broadcasting the joke to all connected clients.

##### Example

To request a joke, send the following message to the subscription socket:
```cpp
axelvanherle>service?>joke
```

To receive the requested joke, subscribe to the following topic:
```cpp
axelvanherle>service!>
```
You will receive a message like this:
```cpp
axelvanherle>service!> <JOKE WILL BE HERE>
```
### Subscription Socket
The subscription socket listens for incoming messages from clients. The server filters incoming messages by checking the topic of the message against the server's subscription topic.

#### Connection Details

*   Type: ZMQ\_SUB
*   Protocol: TCP
*   Address: benternet.pxl-ea-ict.be:24042
*   Subscription Topic: axelvanherle>service?>

## Sending Messages to Clients
The server can send messages to clients by connecting to the push socket and sending messages to it.

### Push Socket
The push socket sends messages to clients. Messages sent through this socket are broadcast to all connected clients.

#### Connection Details

*   Type: ZMQ\_PUSH
*   Protocol: TCP
*   Address: benternet.pxl-ea-ict.be:24041
*   Push Topic: axelvanherle>service!>

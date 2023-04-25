# Documentation

## `src`
If you navigate to this directory you will find extensive documentation over how *my* code works. If you navigate to this directory you will find the `benternet_server` and `benternet_client` subdirectory. In each of these subdirectories is a `readme.md` file which contains all the more detailed documentation.

If you just simply want to know how to interact with my service than you have come to the right place.

For now you can interact with the service on two ways. There is a chat service and a joke service. But what are these? Look below.

### Chat Service

#### Sending
To push messages to the chat server you have to generate a unique ID from 1 - 100000. With this ID you will be identified by other chatters. You can generate a ID using `(rand() % 100000)+1`. A practical example of this could be `playerID = std::to_string((rand() % 100000)+1);`. After you generate your ID you are almost ready to start chatting. The service expects messages in this format: `axelvanherle>service>%YOUR GENERATED ID>chat>%YOUR MESSAGE`. An example packet could look like this: `axelvanherle>service?>22864>chat>Hello world!`.

#### Receiving
Now that you are able to send messages you might also want to be able to receive messages, right? Well to do this the process is simpler. You don't need a generated ID! Hooray. All messages received by the service will be pushed to the `axelvanherle>service!>chat>%MESSAGE` topic. Where `%MESSAGE` is the placeholder for the message that is relayed. So if a message gets pushed by a client, every subscribed client and or service will be receiving these messages. This means that you too can now glow in the dark and listen in on all the juicy private chats. An example message could look like `axelvanherle>service!>chat>Check out this wicked skateboaring trick guys! https://youtu.be/dQw4w9WgXcQ`

### joke service

To be able to use this service you are going to need to generate a unique ID from 1 - 100000. This enables you to only receive jokes when *you* want, and it wont be a cluster of bad jokes spamming you. You can generate a ID using `(rand() % 100000)+1`. A practical example of this could be `playerID = std::to_string((rand() % 100000)+1);`. This ID will be the same to request and receive a joke, so don't lose it.

#### Requesting a (bad) joke
You will need the generated ID for this, so don't skip this step. To request a joke you send `axelvanherle>service?>%YOUR GENERATED ID>joke` into the ether. Just kidding, you send it to the broker. This is all you need to do to request a joke. A practical example of this could look like: `axelvanherle>service?>22869>joke`.

#### Receiving the (bad) joke
You will need the generated ID for this, so don't skip this step. To receive a joke you need to subscribe to `axelvanherle>service!>%YOUR GENERATED ID>joke>%JOKE HERE`. This is all you need to do to receive the requested jokes. A practical example of this could look like: `axelvanherle>service?>22869>joke>I'm afraid for the calendar. Its days are numbered.`.
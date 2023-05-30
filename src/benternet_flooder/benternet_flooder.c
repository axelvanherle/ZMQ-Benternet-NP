#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "zmq.h"

void printIntro(char*, char*, int);

int main(int argc, char *argv[])
{
    // Check if commandline args are correctly passed
    if (argc != 4)
    {
        printf("Usage: ./program <topic> <message> <amount>\n");
        printf("Example: ./program 'axelvanherle>service!>log>' bleh 5\n");
        return 1;
    }

    // Save arguments into variables
    char* topic = argv[1];
    char* message = argv[2];
    int amount = atoi(argv[3]);
    
    printIntro(topic, message, amount);

    // Make context and socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_PUSH);
    assert(socket);
    // Connect the socket
    int rc = zmq_connect(socket, "tcp://benternet.pxl-ea-ict.be:24041");
    assert(rc == 0);

    // Create the base string and copy topic and message into it
    char baseString[128];
    strcpy(baseString, topic);
    strcat(baseString, message);

    // String to store buffer for sending.
    char buffer[128];

    // Flood it
    for (size_t i = 0; i < amount; i++)
    {
        snprintf(buffer, sizeof(buffer), "%s[%zu]", baseString, i);
        rc = zmq_send(socket, buffer, strlen(buffer), 0);
        assert(rc == strlen(buffer));
    }

    // Cleanup
    zmq_close(socket);
    zmq_ctx_destroy(context);
}

void printIntro(char* topic, char* message, int amount)
{
    printf("                                                      \n");
    printf("   (                  )                            )  \n");
    printf(" ( )\\    (         ( /(   (   (             (   ( /(  \n");
    printf(" )((_)  ))\\  (     )\\()) ))\\  )(    (      ))\\  )\\()) \n");
    printf("((_)_  /((_) )\\ ) (_))/ /((_)(()\\   )\\ )  /((_)(_))/  \n");
    printf(" | _ )(_))  _(_/( | |_ (_))   ((_) _(_/( (_))  | |_   \n");
    printf(" | _ \\/ -_)| ' \\))|  _|/ -_) | '_|| ' \\))/ -_) |  _|  \n");
    printf(" (___/\\___||_||_|  \\__|\\___| |_|  |_||_| \\___|  \\__|  \n");
    printf(" )\\ )  (             (                                \n");
    printf("(()/(  )\\            )\\ )   (   (                     \n");
    printf(" /(_))((_) (    (   (()/(  ))\\  )(                    \n");
    printf("(_))_| _   )\\   )\\   ((_))/((_)(()\\                   \n");
    printf("| |_  | | ((_) ((_)  _| |(_))   ((_)                  \n");
    printf("| __| | |/ _ \\/ _ \\/ _` |/ -_) | '_|                  \n");
    printf("|_|   |_|\\___/\\___/\\__,_|\\___| |_|          By AxelV\n\n");
    printf("                                                      \n");
    printf("Flooding %s with %s as message %d times\n",topic, message, amount);
}
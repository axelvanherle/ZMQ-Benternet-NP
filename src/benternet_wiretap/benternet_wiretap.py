import zmq

print(" ______               __                           __   ")
print("|   __ \.-----.-----.|  |_.-----.----.-----.-----.|  |_ ")
print("|   __ <|  -__|     ||   _|  -__|   _|     |  -__||   _|")
print("|______/|_____|__|__||____|_____|__| |__|__|_____||____|")
print("                                                        ")
print(" ________ __              __                            ")
print("|  |  |  |__|.----.-----.|  |_.---.-.-----.             ")
print("|  |  |  |  ||   _|  -__||   _|  _  |  _  |             ")
print("|________|__||__| |_____||____|___._|   __|             ")
print("                                    |__|                ")

userInput = input("Is there a specefic topic you want to subsribe on? Leave empty if you want to subsribe on all: ")

if userInput:
    print(f"Subscribing to {userInput}")
else:
    print("Subscribing to all topics")

# Open the file
file_obj = open("wiretap.txt", "w")

# Define the IP and port
ip = "tcp://benternet.pxl-ea-ict.be"
port = "24042"

# Create a ZeroMQ context
context = zmq.Context()

# Create a subscriber socket
subscriber = context.socket(zmq.SUB)

# Set the socket options
subscriber.setsockopt_string(zmq.SUBSCRIBE, userInput)

# Connect the subscriber to the specified IP and port
subscriber.connect(f"{ip}:{port}")

# Infinite loop to receive and process messages
while True:
    # Receive the message
    message = subscriber.recv_string()

    # Process the message (print it in this example)
    print("Received message:", message)
    file_obj.write(message + '\n')
    file_obj.flush()

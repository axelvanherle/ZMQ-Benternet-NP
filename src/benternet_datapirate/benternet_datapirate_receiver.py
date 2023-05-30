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

# Open the file in read mode to check if message already exists
file_obj = open("wiretap.txt", "r")

# Read the existing messages in the file
existing_messages = set(file_obj.read().splitlines())

# Close the file
file_obj.close()

# Open the file in append mode to add new messages
file_obj = open("wiretap.txt", "a")

# Define the IP and port
ip = "tcp://benternet.pxl-ea-ict.be"
port = "24042"

# Create a ZeroMQ context
context = zmq.Context()

# Create a subscriber socket
subscriber = context.socket(zmq.SUB)

# Set the socket options
subscriber.setsockopt_string(zmq.SUBSCRIBE, "")

# Connect the subscriber to the specified IP and port
subscriber.connect(f"{ip}:{port}")

# Infinite loop to receive and process messages
while True:
    # Receive the message
    message = subscriber.recv_string()

    # Check if the message already exists
    if message not in existing_messages:
        # Process the message (print it in this example)
        print("Received message:", message)
        
        file_obj.write(message + '\n')
        file_obj.flush()

        # Add the message to the existing messages set
        existing_messages.add(message)

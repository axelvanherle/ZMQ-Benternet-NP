import zmq
import random
import time

# Function to read the wiretap.txt file and get random lines
def get_random_line(filename):
    with open(filename, "r") as file:
        lines = [line for line in file if '>' in line]  # Filter lines that contain '>'
        if not lines:  # If there are no such lines, return a default string or handle appropriately
            return "No lines with '>' found."
        return random.choice(lines).strip()
    
# Define the IP and port
ip = "tcp://benternet.pxl-ea-ict.be"
port = "24041"

# Create a ZeroMQ context
context = zmq.Context()

# ZeroMQ PUSH socket setup
context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.connect(f"{ip}:{port}")

# Main program loop
while True:
    line = get_random_line("wiretap.txt")
    socket.send(line.encode())

    time.sleep(0.1)
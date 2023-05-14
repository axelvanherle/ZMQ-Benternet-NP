import zmq

endpoint = "tcp://benternet.pxl-ea-ict.be:24042"

context = zmq.Context()
socket = context.socket(zmq.SUB)

socket.setsockopt_string(zmq.SUBSCRIBE, "axelvanherle>service!>log>")

socket.connect(endpoint)

# Start receiving messages
while True:
    print(socket.recv())

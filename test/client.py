# Simple example of a client that sends a hex string to the server and prints the response
# Author: Zheng Lin Lei

from lib.MemDump import MemDump
from lib.ISO8583Msg import ISOMSG
import socket, sys

if len(sys.argv) > 1 and sys.argv[1] in ISOMSG:
    request = ISOMSG[sys.argv[1]]
else:
    request = ISOMSG["1200"]


# Create socket and connect to server
client = socket.socket()
client.connect(('127.0.0.1', 9101))

# Send request and print response
request = bytes.fromhex(request)
MemDump("Request", request)
client.send(request)

# Receive response and print response
response = client.recv(1024)
MemDump("Response", response)

client.close()
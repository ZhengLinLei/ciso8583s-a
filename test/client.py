# Simple example of a client that sends a hex string to the server and prints the response
# Author: Zheng Lin Lei

from lib.MemDump import MemDump
from lib.ISO8583Msg import ISOMSG
import socket, sys

if len(sys.argv) > 1:
    if sys.argv[1] in ISOMSG:
        request = ISOMSG[sys.argv[1]]
    else:
        print("Avaliable ISO8583 Message")
        for i in ISOMSG.keys(): print(i)
        sys.exit(1)
else:
    request = ISOMSG["1200"]


# Create socket and connect to server
client = socket.socket()
client.connect(('127.0.0.1', 9101))

# Send request and print response
# Header = len/2 -> hex(2 bytes)
request = bytes.fromhex(hex((int(len(request)/2)))[2:].zfill(4) + request)
MemDump("Request", request)
client.send(request)

# Receive response and print response
response = client.recv(1024)
MemDump("Response", response)

client.close()
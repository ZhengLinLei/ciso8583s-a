# Simple example of a client that sends a hex string to the server and prints the response
# Author: Zheng Lin Lei


import socket

hex1200 = "007c12907034051908c1080116888880366790225500000000000000057100005023120200523449124d313031303136353430344302862312020003061365443030353020202020202020203030303030303031202020202020202020202020202020000930303630313838383801020000020000001787b59700000000"


client = socket.socket()
client.connect(('127.0.0.1', 9101))
client.send(bytes.fromhex(hex1200))
print(client.recv(1024).hex())

client.close()
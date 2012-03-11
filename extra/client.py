#!/usr/bin/env python 

import socket 

host = 'flip.engr.oregonstate.edu' 
port = 7331 
size = 255 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((host,port)) 
s.send('Hello, world') 
data = s.recv(size) 
s.close() 
print 'Received:', data

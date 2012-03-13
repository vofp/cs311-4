#!/usr/bin/env python 

import socket 

host = 'os-class.engr.oregonstate.edu' 
port = 7331 
size = 255 
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
client_socket.connect((host,port)) 
client_socket.send('RPT') 
while(data != 'END'):
	data = client_socket.recv(size) 
	print 'Received:', data




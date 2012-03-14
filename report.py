#!/usr/bin/env python 

import socket 

host = 'os-class.engr.oregonstate.edu' 
port = 7331 
size = 255 
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
client_socket.connect((host,port)) 
client_socket.send('RPT 0') 
data = client_socket.recv(size)
index = 1
print "Perfects:"
while(data != 'NIL'):
	data_array = data.split(" ")
	print data_array[1]
	client_socket.send('RPT '+ str(index))
	data = client_socket.recv(size) 
	index += 1



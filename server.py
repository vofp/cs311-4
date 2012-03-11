import socket 

host = '' 
port = 7331
backlog = 5 
size = 255 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((host,port)) 
s.listen(backlog)
check = 2
while 1: 
	client, address = s.accept() 
	data = client.recv(size) 
	if data:
		data_array = data.split(" ")
		if data_array[0] == 'IOPS':
			iops = float(data_array[1])/1000000.0
			divisions = 15/iops*1000000000
			lower = check
			while(divisions > 0):
				divisions -= check
				check += 1
			client.send('From server: '+str(lower) + ' '+ str(check-1)) 
	client.close()


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
		if data_array[0] == 'REQ':
			iops = float(data_array[1])/1000000.0
			divisions = 15/iops*1000000000
			lower = check
			while(divisions > 0):
				divisions -= check
				check += 1
			client.send('RES '+str(lower) + ' '+ str(check-1))
			print 'request from ' + str(lower) + ' to ' + str(check-1)
		elif data_array[0] == 'PFN':
			print 'Perfect: ' + str(data_array[1])
			client.send('ACK PFN')
		elif data_array[0] == 'DON':
			print 'Range Done: ' + str(data_array[1]) + ' to ' + str(data_array[2])
			client.send('ACK DON')
		elif data_array[0] == 'EXT':
			print 'Exit requested'
			client.send('ACK EXT')
		else:
			print 'Unknown data: ' + data
			client.send('NAK')
	client.close()


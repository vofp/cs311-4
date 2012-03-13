import socket
import sys
import select 

host = '' 
port = 7331
backlog = 5 
size = 255 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((host,port)) 
s.listen(backlog)
check = 2
socket_list = [s]
listener_list = []
while 1:
	readable, writeable, error = select.select(socket_list,[],[])
	for client in readable:
		if client is s: 
			new_client, address = s.accept()
			socket_list.append(new_client)
			print 'Connected by', address
		else:
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
				elif data_array[0] == 'SIG':
					listener_list.append(client)
				elif data_array[0] == 'PFN':
					print 'Perfect: ' + str(data_array[1])
					client.send('ACK PFN')
				elif data_array[0] == 'DON':
					print 'Range Done: ' + str(data_array[1]) + ' to ' + str(data_array[2])
					client.send('ACK DON')
				elif data_array[0] == 'CLS':
					print address, ' Closing'
					client.send('ACK CLS')
					client.close()
					socket_list.remove(client)
					break
				elif data_array[0] == 'EXT':
					print 'Exit requested'
					client.send('ACK EXT')
					sys.exit()
				else:
					print 'Unknown data: ' + data
					client.send('NAK')
			else:
				client.close()
				socket_list.remove(client)

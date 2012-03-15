import socket
import sys
import select 
import signal

host = '' 
port = 7331
backlog = 5 
size = 255 
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server_socket.bind((host,port)) 
server_socket.listen(backlog)
perfect_numbers = []
check = 2
socket_list = [server_socket]
listener_list = []

def close(signum,fname):
	for l in listener_list:
		l.send('EXT')
		l.close()
	server_socket.close()
	sys.exit()
	
signal.signal(signal.SIGTERM,close)
signal.signal(signal.SIGINT,close)


while 1:
	readable, writeable, error = select.select(socket_list,[],[])
	for client in readable:
		if client is server_socket: 
			new_client, address = server_socket.accept()
			socket_list.append(new_client)
			print 'Connected by', address
		else:
			data = client.recv(size) 
			if data:
				data_array = data.split(" ")
				if data_array[0] == 'REQ':
					print data
					#iops = float(data_array[1])/1000000.0
					divisions = int(data_array[1])
					print divisions
					lower = check
					#while(divisions > 0):
					#	divisions -= check
					#	check += 1
					#check = int((2 * divisions - lower - lower **2 ) ** 0.5)+1
					check += int((2*divisions+2)**0.5)
					client.send('RES '+str(lower) + ' '+ str(check-1))
					print 'request from ' + str(lower) + ' to ' + str(check-1)
				elif data_array[0] == 'SIG':
					listener_list.append(client)
				elif data_array[0] == 'PFN':
					perfect_numbers.append(data_array[1])
					print perfect_numbers
					print 'Perfect: ' + str(data_array[1])
					client.send('ACK PFN')
				elif data_array[0] == 'RPT':
					index = int(data_array[1])
					if index < len(perfect_numbers):
						client.send('RES '+ perfect_numbers[index])
					else:
						client.send('NIL')
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
					for l in listener_list:
						l.send('EXT')
						l.close()
					sys.exit()
				else:
					print 'Unknown data: ' + data
					client.send('NAK')
			else:
				client.close()
				socket_list.remove(client)

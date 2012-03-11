import socket 

host = '' 
port = 7331
backlog = 5 
size = 255 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((host,port)) 
s.listen(backlog) 
while 1: 
    client, address = s.accept() 
    data = client.recv(size) 
    if data: 
        client.send(data+" - from server") 
    client.close()


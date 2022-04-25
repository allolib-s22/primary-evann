from socket import *
serverName = 'localhost'
serverPort = 9010
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = input("Input lowercase sentence:")
clientSocket.sendto(message.encode(),(serverName, serverPort))
clientSocket.close()
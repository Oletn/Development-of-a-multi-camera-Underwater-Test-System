# client.py
import socket

# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the port on which you want to connect
port = 8080

# Connect to the server on local computer
s.connect(('192.168.123.4', port))  # Replace '192.168.1.2' with the IP address of the server Jetson Nano

# Receive data from the server
print(s.recv(1024).decode())

# Close the connection
s.close()
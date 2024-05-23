import cv2
import base64
import sys
import serial
import socket
import subprocess
from flask import Flask, send_from_directory
from flask_socketio import SocketIO
from threading import Thread, Timer

import logging

# If one needs to use the pins on the jetson nano: import Jetson.GPIO as GPIO
#GPIO.setwarnings(False)

sys.path.append("/usr/local/lib/python3.6/dist-packages")

app = Flask(__name__)
socketio = SocketIO(app)


#log = logging.getLogger('werkzeug') #Flask logger was disabled to avoid the log messages in the console, it was used during programming to debug
#log.setLevel(logging.WARNING) #only important messages will be shown in the console


#Get access to serial port on the Jetson Nano. Then call the serial port and set the baudrate to 115200
subprocess.call(["sudo", "chmod", "777", "/dev/ttyACM0"])
ser = serial.Serial('/dev/ttyACM0', 115200)

# Read the indexnew.html file for the web interface to the client
with open('/home/jetson/Desktop/Dynamixel_Python/webapp/indexNew.html', 'r') as file:
    html = file.read()

#static files sent for serving the socketIO on the client side
@app.route('/')
def index(): #index function to send the indexNew.html file to the client
    return html 

@app.route('/static/<path:path>') #any URL that starts with /static/ followed by any path will be sent to the send_js function
def send_js(path): #file with path requested by indexNew.html will be sent
    return send_from_directory('static', path) #Flask function to send files from a directory

#Socket communication between the Jetson Nano server and the client for handeling camera feed, motor control, temperature, humidity, water leakage and window size
@socketio.on('motorButtons')
def handle_motorbuttons(data):
    variable_name = "status"
    value_char = int(data['status'])
    data_to_send = f"VAR:{variable_name}, VAL:{value_char}\n"
    print('thisIsSendt:', data_to_send)
    ser.write(bytes(data_to_send, 'utf-8'))


@socketio.on('window')
def windowSize(data):
    a = data['a']
    b = data['b']
    #print('window size:', data)
    variable_name = "window_size"
    window_data_to_send = f"VAR:{variable_name}, VAL:{a},{b}\n"
    ser.write(bytes(window_data_to_send, 'utf-8'))

@socketio.on('move_motor') 
def handle_move_motor(data):
    x = round(data['x'])
    y = round(data['y'])
    #print('x:', x, 'y:', y)
    data_to_send = f"VAR:Touch, VAL:{x},{y}\n"
    ser.write(bytes(data_to_send, 'utf-8'))

@socketio.on('shutdown')
def handle_shutdown(data):
    os.system("sudo shutdown now")



#Background threads for reading the serial port and sending the data to the client html page
def listen_to_serial():
    while True:
        data = ser.readline().decode().strip()
        #print('data from RB150:', data)
        #if "shutdown" in data:
        # os.system("sudo shutdown now")
        if "Humidity from DHT: " in data:
            humidity = data.split(": ")[-1]
            socketio.emit('humidity', humidity)
        if "Temperature from DHT: " in data:
            temperature = data.split(": ")[-1]
            socketio.emit('temperatureDHT', temperature)
        if "Water leakage sensor value: " in data:
            water_leakage = data.split(": ")[-1]
            socketio.emit('water_leakage', water_leakage)

Thread(target=listen_to_serial).start()

#Background threads for sending the camera feed and the Jetson Nano temperature to the client html page
@socketio.on('frame')
def frames():
    camera = cv2.VideoCapture(0)
    while True:
        success, frame = camera.read()
        if not success:
            break
        success2, buffer = cv2.imencode('.jpg', frame)
        if not success2:
            break
        frame = base64.b64encode(buffer).decode('utf-8')#buffer.tobytes()
        socketio.emit('frame', frame)
 
Thread(target=frames).start()

def jetsontemp():
    temp = subprocess.check_output(["cat", "/sys/devices/virtual/thermal/thermal_zone0/temp"])
    temp = int(temp) / 1000.0
    #print(temp)
    socketio.emit('jetsontemp', temp)
    Timer(10, jetsontemp).start()

Thread(target=jetsontemp).start()


if __name__ == "__main__":
    socketio.run(app, host='0.0.0.0', port=3000, allow_unsafe_werkzeug=True) 
    #Unsafe so the server will run on the Jetson Nano
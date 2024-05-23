# Here all the files containing the code for the project are listed.


The DYnamixel_Python folder contains software for running the inspection camera system. It contains a python flask server whitch is the center of cimmunication in the software. The server also have a webpage coded in HTML and prettified with CSS to display a live video stream, importent sensor readings and control two servo motor. A script for an arduni microcontroller is also in this folder, this scripts read sensor data and control the DYNAMIXEL servos. The different scripts, microcontrollers and SBC´s communicate via serial, and with socket.io to the client.

The LoRa communication folder contains two scripts for the Adafruit Feather M0 with RFM95. One of scripts is ment for a feather equipped with an Oled screen. In the screen there is a meny where one can send messages, check connection strength and a user manual. The other script listens for the messages sendt from the other feather, and turns a MOSFET switch on and off. 

The matlab folder is a collection folder containing multiple script with different calculations done throughout the project.

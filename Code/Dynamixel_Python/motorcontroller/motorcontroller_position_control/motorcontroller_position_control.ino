/* Includes the needed libraries */
#include <Dynamixel2Arduino.h> 
#include <DHT.h>
#include <math.h>

// defenition of the DHT sensor
#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Defenition of the Dynamixel servos
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;
const uint8_t DXL_1 = 1;
const uint8_t DXL_2 = 2; 
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
const float DXL_PROTOCOL_VERSION = 2.0;

// Defenition of the moisture sensor
int sensordata = 0;
int prevsensordata =0;

// Defenition of the DHT sensor
int humidity = 0;
int temperatureC = 0;
int prevhumidity = 0;
int prevtemp = 0;
float value = 0.0; 
float orient = -1.0;
float deltaX = 0; 
float deltaY = 0;

// Defenition of the window size
int windowWidth = 800;
int windowHeight = 600;

// Defenition of the water leakage sensor
#define SIGNAL_WATWER A0
int WaterLeakagevalue = 0; 
int previousWaterLeakageValue = -1;


using namespace ControlTableItem; // Control table item namespace for DYNAMIXEL servos and sensors




void setup(){
  delay(1000);
  DEBUG_SERIAL.begin(115200); // Start the serial communication between the arduino and the jetson nano
  DXL_SERIAL.begin(9600); // Start the serial communication for the dynamixel servos
  dxl.begin(57600); // Start the dynamixel servos communication with the arduino
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION); // Set the protocol version for the dynamixel servos
  
  dht.begin(); // Start the DHT sensor


//a function for setting up the dynamixel servos in correct format and checking if they are connected
 dynamixelconnection();
 
 //Get the servos to the home position
  gethome();

  delay(1000);
}

void loop() {
int currentPosition1 = dxl.getPresentPosition(DXL_1); // Get the current position of the first dynamixel servo
int currentPosition2 = dxl.getPresentPosition(DXL_2); // Get the current position of the second dynamixel servo

  if (value > 0 && value < 6)  
    motorkontrollButtons(value, currentPosition1, currentPosition2, orient);

  if (DEBUG_SERIAL.available() > 0) {
    char data[50]; // Create a character array to store the data
    DEBUG_SERIAL.readStringUntil('\n').toCharArray(data, 50); // Read data into character array
    String dataStr = String(data); // Convert character array to string
    
    
    if (dataStr.startsWith("VAR:")) { // Check if the data string starts with "VAR:"
            String variable_name = dataStr.substring(4, dataStr.indexOf(",")); // Get the variable name
            String value_str = dataStr.substring(dataStr.indexOf("VAL:") + 4); // Get the value string
            value = value_str.toFloat(); // Convert the value string to a float
        
        if(variable_name =="window_size"){ // Check if the variable name is "window_size"
          int delimiter_pos = value_str.indexOf(","); // Find the position of the delimiter
          windowWidth = value_str.substring(0, delimiter_pos).toInt(); // Get the width of the window
          windowHeight = value_str.substring(delimiter_pos + 1).toInt();  // Get the height of the window
        }

        if (value == 6){ // Check if the value sent from is 6 and changes the orientation of the dynamixel servos on the y-axis
          if (orient == -1)
            orient = 1; 
          else if (orient == 1)
            orient = -1; 
        }
        
        
        if (variable_name == "Touch") { // Check if the variable name is "Touch"
        int delimiter_pos = value_str.indexOf(","); // Find the position of the delimiter
        int x = value_str.substring(0, delimiter_pos).toInt(); // Get the x value
        int y = value_str.substring(delimiter_pos + 1).toInt(); // Get the y value
        touchcontroll(x, y, currentPosition1, currentPosition2, orient, windowWidth, windowHeight); // Call the touchcontroll function
      }
    }
  }

  //Read the moisture and leakage sensors and print the values to the serial monitor
  readmoisture(); 
  readleakage(); 

  
}


void motorkontrollButtons(int value, int currentPosition1, int currentPosition2, int orient)
{ 
  //DEBUG_SERIAL.println(String("Dette er switch case verdien: ") + value);
  //delay(10);
  switch(value) { // Switch case for the different camera movements
    case 1: 
      dxl.setGoalPosition(DXL_1, currentPosition1+50);
      break; 
    
    case 2: 
      dxl.setGoalPosition(DXL_1, currentPosition1-50);
      break;
    
    case 3: 
      dxl.setGoalPosition(DXL_2, currentPosition2-50*orient);
      break;

    case 4: 
      dxl.setGoalPosition(DXL_2, currentPosition2+50*orient);
      break;

    case 5: 
      dxl.setGoalPosition(DXL_1, 1010);
      dxl.setGoalPosition(DXL_2, 1000);
      break; 

    default:
      break; 
  }
}

void touchcontroll(int x, int y, int currentPosition1, int currentPosition2, int orient, int windowWidth, int windowHeight) { 

  float test1 = (windowWidth/2); // Calculate the middle  width
  float test2 = (windowHeight/2); // Calculate the middle height

  float distancePercentageX = x/test1; // Calculate the distance percentage on the "x-axis"
  float distancePercentageY = y/test2; // Calculate the distance percentage on the "y-axis"

  if (distancePercentageX > 1)
    deltaX = (distancePercentageX-1)*30;
  else if (distancePercentageX < 1)
    deltaX = (1-distancePercentageX)*-30; 
  else
    deltaX = 0;
  
  if (distancePercentageY > 1)
    deltaY = (distancePercentageY-1)*-30;
  else if (distancePercentageY < 1)
    deltaY = (1-distancePercentageY)*30;
  else
    deltaY = 0; 
  
  float Position1 = dxl.getPresentPosition(DXL_1, UNIT_DEGREE)+deltaX;
  float Position2 = dxl.getPresentPosition(DXL_2, UNIT_DEGREE)+deltaY*orient*(-1);

  dxl.setGoalPosition(DXL_1, Position1, UNIT_DEGREE);
  dxl.setGoalPosition(DXL_2, Position2, UNIT_DEGREE);
}

void readmoisture(){
  humidity = dht.readHumidity(); // Read the humidity from the DHT sensor
  temperatureC = dht.readTemperature(); // Read the temperature in Celsius from the DHT sensor

DEBUG_SERIAL.println("Humidity from DHT: " + String(humidity));
DEBUG_SERIAL.println("Temperature from DHT: " + String(temperatureC));

/* If problems with too much serial communication activiate the functions under and remove the two functions over

  if (humidity != prevhumidity && humidity != NAN) {
      DEBUG_SERIAL.println("Humidity from DHT: " + String(humidity));
      prevhumidity = humidity;
  }

  if(temperatureC != prevtemp && temperatureC != NAN){
     DEBUG_SERIAL.println("Temperature from DHT: " + String(temperatureC));
     prevtemp = temperatureC;
  }
*/
}

void readleakage(){
  int currentWaterLeakageValue =  analogRead(SIGNAL_WATWER); 
  DEBUG_SERIAL.println("Water leakage sensor value: " + String(currentWaterLeakageValue));

  //Use this code if you want to send fewer serial messages
  /*if(currentWaterLeakageValue != previousWaterLeakageValue && abs(currentWaterLeakageValue - previousWaterLeakageValue) >= 10) {
    DEBUG_SERIAL.println("Water leakage sensor value: " + String(currentWaterLeakageValue));
    previousWaterLeakageValue = currentWaterLeakageValue;
    if(currentWaterLeakageValue > 200){
      DEBUG_SERIAL.println("shutdown");
    }
  }*/
}


void gethome(){
  int currentPositionStart = dxl.getPresentPosition(DXL_1); // Get the current position of the first dynamixel servo
  
  if(currentPositionStart > 2900){
    dxl.torqueOff(DXL_1);
    dxl.torqueOff(DXL_2);
    dxl.setOperatingMode(DXL_1, OP_VELOCITY);
    dxl.torqueOn(DXL_1);

    dxl.setGoalVelocity(DXL_1, 30);

    delay(2000);
    dxl.torqueOff(DXL_1);
    dxl.torqueOff(DXL_2);
  }
  
  int minPosition1 = -33; 
  int maxPosition1 = 2000; 
  int minPosition2 = 55; 
  int maxPosition2 = 2000; 

  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  dxl.setOperatingMode(DXL_1, OP_POSITION);
  dxl.setOperatingMode(DXL_2, OP_POSITION);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_1, 25);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_2, 25); 
  dxl.writeControlTableItem(MIN_POSITION_LIMIT, DXL_1, minPosition1); // Set the minimum position limit for the first dynamixel servo
  dxl.writeControlTableItem(MAX_POSITION_LIMIT, DXL_1, maxPosition1); // Set the maximum position limit for the first dynamixel servo
  dxl.writeControlTableItem(MIN_POSITION_LIMIT, DXL_2, minPosition2); // Set the minimum position limit for the second dynamixel servo
  dxl.writeControlTableItem(MAX_POSITION_LIMIT, DXL_2, maxPosition2); // Set the maximum position limit for the second dynamixel servo
  dxl.torqueOn(DXL_1);
  dxl.torqueOn(DXL_2);
 

  dxl.setGoalPosition(DXL_1, 1010);
  dxl.setGoalPosition(DXL_2, 1000);
}

void dynamixelconnection(){
  // Get DYNAMIXEL connection
  if (dxl.ping(DXL_1)) {
    DEBUG_SERIAL.print("Servo ");
    DEBUG_SERIAL.print(DXL_1);
    DEBUG_SERIAL.println(" connected.");
  }
  else {
    DEBUG_SERIAL.print("Servo ");
    DEBUG_SERIAL.print(DXL_1);
    DEBUG_SERIAL.println(" failure.");
  }
  if (dxl.ping(DXL_2)) {
    DEBUG_SERIAL.print("Servo ");
    DEBUG_SERIAL.print(DXL_2);
    DEBUG_SERIAL.println(" connected.");
  }
  else {
    DEBUG_SERIAL.print("Servo ");
    DEBUG_SERIAL.print(DXL_2);
    DEBUG_SERIAL.println(" failure.");
  }
}
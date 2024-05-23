#include <Dynamixel2Arduino.h>
#include <DHT.h>
#include <math.h>
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int humidity = 0;
int temperatureC = 0;
int prevhumidity = 0;
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;
const uint8_t DXL_1 = 1;
const uint8_t DXL_2 = 2; 
int velocityInit = 30;
char value = '0';
char prevValue = '0';
int sensordata = 0;
int prevsensordata =0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
const float DXL_PROTOCOL_VERSION = 2.0;
using namespace ControlTableItem;

void setup()
{
 
  delay(1000);
  DEBUG_SERIAL.begin(115200);
  DXL_SERIAL.begin(9600);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
 
  // Get DYNAMIXEL information
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
  

  int currentPositionStart = dxl.getPresentPosition(DXL_1);
  if(currentPositionStart > 2900){
  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  dxl.setOperatingMode(DXL_1, OP_VELOCITY);
  dxl.torqueOn(DXL_1);
  dxl.setGoalVelocity(DXL_1, 40);
  delay(3000);
  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  }
  
  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  dxl.setOperatingMode(DXL_1, OP_POSITION);
  dxl.setOperatingMode(DXL_2, OP_POSITION);
  dxl.torqueOn(DXL_1);
  dxl.torqueOn(DXL_2);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_1, 30);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_2, 30);

 

  dxl.setGoalPosition(DXL_1, 1010);
  dxl.setGoalPosition(DXL_2, 1000);

  delay(6000);
  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  
  dxl.ledOn(DXL_1);

  delay(1000); 
  
  dxl.torqueOff(DXL_1);
  dxl.torqueOff(DXL_2);
  dxl.setOperatingMode(DXL_1, OP_VELOCITY);
  dxl.setOperatingMode(DXL_2, OP_VELOCITY);
  dxl.torqueOn(DXL_1);
  dxl.torqueOn(DXL_2);
  
  delay(1000);

  dxl.ledOn(DXL_1);
}

void loop() {
  int minPosition1 = -33; 
  int maxPosition1 = 2000; 
  int minPosition2 = 55; 
  int maxPosition2 = 2000; 
/*
  if (currentPosition1 < minPosition1) {
    dxl.torqueOff(DXL_1);
    dxl.setOperatingMode(DXL_1, OP_POSITION);
    dxl.torqueOn(DXL_1);
    dxl.setGoalPosition(DXL_1, minPosition1+10);
    dxl.torqueOff(DXL_1);
    dxl.setOperatingMode(DXL_1, OP_VELOCITY);
    
  } else if (currentPosition1 > maxPosition1) {
    dxl.torqueOff(DXL_1);
    dxl.setOperatingMode(DXL_1, OP_POSITION);
    dxl.torqueOn(DXL_1);
    dxl.setGoalPosition(DXL_1, maxPosition1-10);
    dxl.torqueOff(DXL_1);
    dxl.setOperatingMode(DXL_1, OP_VELOCITY);
  }

  if (currentPosition2 < minPosition2) {
    dxl.torqueOff(DXL_2);
    dxl.setOperatingMode(DXL_2, OP_POSITION);
    dxl.torqueOn(DXL_2);
    dxl.setGoalPosition(DXL_2, minPosition2+10);
    dxl.torqueOff(DXL_2);
    dxl.setOperatingMode(DXL_2, OP_VELOCITY);

  } else if (currentPosition2 > maxPosition2) {
     dxl.torqueOff(DXL_2);
    dxl.setOperatingMode(DXL_2, OP_POSITION);
    dxl.torqueOn(DXL_2);
    dxl.setGoalPosition(DXL_2, maxPosition2-10);
    dxl.torqueOff(DXL_2);
    dxl.setOperatingMode(DXL_2, OP_VELOCITY);
  } */


  if (DEBUG_SERIAL.available() > 0) {
    char data[50];
    DEBUG_SERIAL.readStringUntil('\n').toCharArray(data, 50); // Read data into character array
    String dataStr =String(data);
    
    
    if (dataStr.startsWith("VAR:")) {
            String variable_name = dataStr.substring(4, dataStr.indexOf(","));
            String value_str = dataStr.substring(dataStr.indexOf("VAL:") + 4);
            float value = value_str.toFloat();

      if (variable_name == "status") {
        int currentPosition1 = dxl.getPresentPosition(DXL_1);
        int currentPosition2 = dxl.getPresentPosition(DXL_2);
        motorkontrollButtons(value, currentPosition1, currentPosition2, minPosition1, maxPosition1, minPosition2, maxPosition2);

      } else if (variable_name == "Touch") {
        int delimiter_pos = value_str.indexOf(",");
        
        int x = value_str.substring(0, delimiter_pos).toInt();
        int y = value_str.substring(delimiter_pos + 1).toInt();
        
        touchcontroll(x, y, currentPosition2, currentPosition2);
      
      }
    }
  }
  //readmoisture();
}

void motorkontrollButtons(int value, int currentPosition1, int currentPosition2, int minPosition1, int  maxPosition1, int minPosition2,  int maxPosition2)
{

  switch (value) {
    case 1:
      if(currentPosition1 >= maxPosition1)
        break;
      else if (currentPosition1 <= maxPosition1)
        dxl.setGoalVelocity(DXL_1, velocityInit);
      break;

    case 2:
      if(currentPosition1 <= minPosition1)
        break;
      else if(currentPosition1 >= minPosition1)
        dxl.setGoalVelocity(DXL_1, -velocityInit);
      break;

    case 3:
      if(currentPosition2 >= maxPosition2)
        break;
      else if(currentPosition2 <= maxPosition2)
        dxl.setGoalVelocity(DXL_2, velocityInit);
      break;

    case 4:
      if(currentPosition2 <= minPosition2)
        break;
      else if (currentPosition2 >= minPosition2)
        dxl.setGoalVelocity(DXL_2, -velocityInit); 
      break;

    default:
      dxl.setGoalVelocity(DXL_1, 0);
      dxl.setGoalVelocity(DXL_2, 0);
      break;
  }
}




void readmoisture(){
  humidity = dht.readHumidity();
  temperatureC = dht.readTemperature();
  if (humidity != prevhumidity && humidity != NAN) {
      DEBUG_SERIAL.println(humidity);
      //DEBUG_SERIAL.println(temperatureC);
      prevhumidity = humidity;
    }}


void touchcontroll(int x, int y, int currentPosition1,  int currentPosition2) {

  int deltaX = 400 - x; // Invert x-axis
  int deltaY = 300 - y; // Invert y-axis

  float distancePercentageX = abs(deltaX) / 400.0; // Assuming image width is 800 pixels
  float distancePercentageY = abs(deltaY) / 300.0; // Assuming image height is 600 pixels

  float goX =  (deltaX > 0 ? 1 : -1) * distancePercentageX;
  float goY = * (deltaY > 0 ? 1 : -1) * distancePercentageY;
  
  newPosition1 = currentPosition1 + round(goX)
  newPosition2 = currentPosition2 + round(goY)

  int minPosition1 = -33; 
  int maxPosition1 = 2000; 
  int minPosition2 = 55; 
  int maxPosition2 = 2000; 

  if ((velocityX > 0 && currentPosition1 < maxPosition1) || (velocityX < 0 && currentPosition1 > minPosition1)) {
    dxl.setGoalVelocity(DXL_1, velocityX*8); // Adjust for horizontal movement
  }

  if ((velocityY > 0 && currentPosition2 < maxPosition2) || (velocityY < 0 && currentPosition2 > minPosition2)) {
    dxl.setGoalVelocity(DXL_2, velocityY*8); // Adjust for vertical movement
  }

  dxl.setGoalVelocity(DXL_1, 0);
  dxl.setGoalVelocity(DXL_2, 0);
}


/*float get_value(){
  if (DEBUG_SERIAL.available() > 0) {
    char data[50];
    DEBUG_SERIAL.readStringUntil('\n').toCharArray(data, 50); // Read data into character array
    String dataStr =String(data);
    
    
    if (dataStr.startsWith("VAR:")) {
            String variable_name = dataStr.substring(4, dataStr.indexOf(","));
            String value_str = dataStr.substring(dataStr.indexOf("VAL:") + 4);
            float value = value_str.toInt();
            return value;
}}}*/


                                                                                                          
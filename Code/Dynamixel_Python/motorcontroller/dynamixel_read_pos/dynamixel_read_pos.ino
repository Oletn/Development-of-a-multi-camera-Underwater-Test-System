//Script to read max postion vaues of dynamixel engines for limitations

#include <Dynamixel2Arduino.h>

#include <math.h>


#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1;
const uint8_t DXL_1 = 1;
const uint8_t DXL_2 = 2; 

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
  
  dxl.writeControlTableItem(MIN_POSITION_LIMIT, DXL_1, -33);
  dxl.writeControlTableItem(MAX_POSITION_LIMIT, DXL_1, 2000);

  
  dxl.writeControlTableItem(MIN_POSITION_LIMIT, DXL_2, 55);
  dxl.writeControlTableItem(MAX_POSITION_LIMIT, DXL_2, 2000);

  int currentPosition =dxl.getPresentPosition(DXL_1);

  if(currentPosition > 2900){
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
}
void loop() {
  
  read_position();
}


int lastPos1 = 0;
int lastPos2 = 0;

void read_position() {
  int pos1 = dxl.getPresentPosition(DXL_1);
  int pos2 = dxl.getPresentPosition(DXL_2);

  if (pos1 != lastPos1) {
    DEBUG_SERIAL.println("Position of Motor DXL_1: " + String(pos1));
    lastPos1 = pos1;
  }

  if (pos2 != lastPos2) {
    DEBUG_SERIAL.println("Position of Motor DXL_2: " + String(pos2));
    lastPos2 = pos2;
  }
}
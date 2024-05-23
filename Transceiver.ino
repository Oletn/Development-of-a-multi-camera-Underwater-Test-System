#include <Wire.h>
#include <RH_RF95.h>

//********* Code is based on exapmple codes from the RadioHead git repor: https://github.com/adafruit/RadioHead ********** //

/***** LoRa Feather ******/
  // define frequnecy for the LoRa module
  #define RF95_FREQ 434.0
  #define RFM95_CS    8
  #define RFM95_INT   3 //Datablad sier 7 men bibliotek sier 3 
  #define RFM95_RST   4
  #define LED        13
  #define Switch     14
  // Singleton instance of the radio driver
  RH_RF95 rf95(RFM95_CS, RFM95_INT);  

/***** Global variables ******/

int16_t packetnum = 0;  // packet counter, we increment per xmission

void setup() {

  //delay(500);
  //Serial.begin(115200);
  //while (!Serial) delay(1); // wait until serial console is open, remove if not tethered to computer

  //Serial.println("Serial is initialized");
  //Serial.println("128x64 OLED FeatherWing test");
  //delay(500); // wait for the OLED to power up

  pinMode(LED, OUTPUT);
  pinMode(Switch, OUTPUT); 
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //Serial.println("Feather LoRa RX/TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    //Serial.println("LoRa radio init failed");
    //Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  //Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    //Serial.println("setFrequency failed");
    while (1);
  }

  //****** Calculating the legal effective radiated power after EN 300 220-2 standard *******// 

//  ERPmW = 10; %dBm
//  antennaGain = 0.7 - 2.15; %dBd
//  attenuationFactor = 93/100; %dB/m 
//  cableLength = 0.15; %m 
//  cableLoss = attenuationFactor * cableLength; %dB
//  txPower = ERPmW-antennaGain+cableLoss
//  Result = 11.5895 dBm
//  Bandwith is by default 125 kHz in the Radiohead Lib

  rf95.setTxPower(11, false);

  //Serial.print("LoRa radio @"); Serial.print((int)RF95_FREQ); Serial.println(" MHz");

  //delay(500); 
}

void loop() {
  /*** recieving function ***/
 if (rf95.available()) {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (! rf95.recv(buf, &len)) {
      //Serial.println("Receive failed");
      return;
    }
    digitalWrite(LED, HIGH);
    delay(1000);
    RH_RF95::printBuffer("Received: ", buf, len);
    //Serial.print("Got: "); Serial.println((char*)buf);
    //Serial.print("RSSI: "); Serial.println(rf95.lastRssi(), DEC);
    
    char radiopacket[20]; 
    strncpy(radiopacket, (char*)buf, len);
    radiopacket[len] = '\0'; // Null-terminate the string
    char test = radiopacket[8];
    if (test == 'A') {
      //Serial.println("Power is ON");
      digitalWrite(Switch, HIGH);
    }
    else if (test == 'B') {
      //Serial.println("Power is OFF");
      digitalWrite(Switch, LOW);
    }
    delay(100); 
    radiopacket[9] = 0;
    delay(100); 
    //Serial.print("Sending "); Serial.println(radiopacket);
    delay(500); 

    rf95.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf95.waitPacketSent();
    digitalWrite(LED, LOW);
  }
  delay(10);
}

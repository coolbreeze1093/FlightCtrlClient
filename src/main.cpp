#include <Arduino.h>
#include "MsgProcess.h"
#include "EspNewMan.h"
#include "PowerCalculation.h"
//12 13 14 15 4 2 
const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 66);
const unsigned int remotePort = 25066;

PowerCalculation m_powerCal(34);
SbcInterface*msgP =new MsgProcess(&m_powerCal);


void setup() {
  Serial.begin(115200);
  msgP->setChannel(0);
  msgP->setEncrypt(false);
  uint8_t macAddr[] = {0xC0,0x49,0xEF,0xB4,0x3A,0x30};
  msgP->setMacAddr(macAddr);
  if(EspNewMan_H.init())
  {
    EspNewMan_H.creatPeer(msgP);
  }
 
}

void loop() {
  msgP->run();

  delay(1000);
}



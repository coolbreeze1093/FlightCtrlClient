#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>
#include <ESP32PWM.h>
#include "ArduinoJson.h"

Servo myservo;  // 创建舵机对象
Servo myservo2;  // 创建舵机对象
ESP32PWM pwm1;

const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 66);
const unsigned int remotePort = 25066;

bool m_isOpen=false;

float maxValue=4095;

WiFiUDP udp;

void parseControlData(uint8_t* data) {
  int16_t value = (data[3] << 8) | data[4];
  Serial.print("Value: ");
  Serial.println(value);
  if(!m_isOpen)
  {
    return;
  }
  float _HDLValue=0,_HDRValue=0,_ThrottleValue=0;
  switch (data[2]) {
    case 0xA0:
    {
      _ThrottleValue = value/(maxValue-ThrottleButtonInitValue);
      
      Serial.println("set ThrottleButton data:");
      Serial.println(_ThrottleValue);
    }
      break;
    case 0xA1:
    {
      if(value>HorizenDirectionButtonLInitValue)
      {
        _HDLValue = value/4096*90+90;
      }
      else{
        _HDLValue = value/HorizenDirectionButtonLInitValue*90;
      }
    }
      
      Serial.println("set HorizenDirectionButtonL data:");
      Serial.println(_HDLValue);
      break;
    case 0xA2:
    {
      if(value>VerticalDirectionButtonInitValue)
      {
        float _value = value/4096;
        _HDRValue = _HDLValue = _value*90+90;
      }
      else{
        float _value = value/VerticalDirectionButtonInitValue;
        _HDRValue = _HDLValue = _value*90;
      }
    }
      
      Serial.println("set VerticalDirectionButton data");
      Serial.println(_VDValue);
      break;
    case 0xA3:
    {
      if(value>HorizenDirectionButtonRInitValue)
      {
        _HDRValue = value/4096*90+90;
      }
      else{
        _HDRValue = value/HorizenDirectionButtonRInitValue*90;
      }
    }
      Serial.println("set HorizenDirectionButtonR data");
      Serial.println(_HDRValue);
      break;
    default:
      Serial.println("Unknown control data type");
      break;
  }
  float _myservo2Value=0,_myservoValue=0,_pwm1Value=0;
  
  if(_pwm1Value>1)
  {
    _pwm1Value=1;
  }
  else if(_pwm1Value<0){
    _pwm1Value=0;
  }
  else{
    _pwm1Value = _ThrottleValue;
  }
  myservo2.write(_myservo2Value);
  myservo.write(_myservoValue);
  pwm1.writeScaled(_pwm1Value);
}

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  WiFi.disconnect();
  Serial.begin(115200);
  

  IPAddress localIP;localIP.fromString("10.0.0.68");
  IPAddress gateway;gateway.fromString("10.0.0.1");
  IPAddress ipMask;ipMask.fromString("255.255.255.0");

  // 设置静态IP地址
  if (!WiFi.config(localIP, gateway, ipMask)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pwm1.attachPin(4,200,10);

  myservo.attach(15);
  myservo2.attach(2);

  Serial.println("Connected to WiFi");
  udp.begin(25068);

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    pwm1.writeScaled(0);
    myservo2.write(90);
    myservo.write(90);
    Serial.println("WiFi connection lost, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    
    delay(1000);
    
    return; 
  }

  int packetSize = udp.parsePacket();
  if (packetSize) {
    uint8_t incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
  myservo2.write(_myservo2Value);
  myservo.write(_myservoValue);
  pwm1.writeScaled(_pwm1Value);
  }
  delay(40);
}



#pragma once
#include "Arduino.h"

class PowerCalculation
{
private:
    int m_powerPin=0;
    const int m_detMin = 142;
    const int m_detMax = 3171;

    const float m_batteryMaxVol = 4.2;

    int16_t m_volatileLast=0;
  
public:
    PowerCalculation(int pin);
    ~PowerCalculation();
    int getRemainPower();
};



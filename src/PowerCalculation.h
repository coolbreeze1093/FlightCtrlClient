#pragma once
#include "Arduino.h"
#include "AverageFilter.h"

class PowerCalculation
{
private:
    int m_powerPin=0;
    const int m_detMin = 142;
    const int m_detMax = 3171;

    const float m_batteryMaxVol = 4.2;
    const float m_batteryMinVol = 3.3;

    float m_volatileLast=0;

    AverageFilter m_AverageFilter;
  
public:
    PowerCalculation(int pin);
    ~PowerCalculation();
    int getRemainPower();
};



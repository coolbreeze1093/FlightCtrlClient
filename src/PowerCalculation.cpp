#include "PowerCalculation.h"


PowerCalculation::PowerCalculation(int pin):m_powerPin(pin),m_AverageFilter(10)
{
}

PowerCalculation::~PowerCalculation()
{
}
int PowerCalculation::getRemainPower()
{
    int16_t _volatile = analogReadMilliVolts(m_powerPin);
    _volatile = m_AverageFilter.update(double(_volatile));
    //Serial.println(_volatile);
    if(abs(_volatile-m_volatileLast)>5)
    {
        m_volatileLast=_volatile;
    }
    int _vol = (m_volatileLast*0.002-m_batteryMinVol+0.24)/(m_batteryMaxVol-m_batteryMinVol)*100;
    if(_vol<0)
    {
        _vol=0;
    }
    else if(_vol>100){
        _vol=100;
    }
    return _vol;
}

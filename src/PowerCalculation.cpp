#include "PowerCalculation.h"


PowerCalculation::PowerCalculation(int pin):m_powerPin(pin)
{
}

PowerCalculation::~PowerCalculation()
{
}
int PowerCalculation::getRemainPower()
{
    int16_t _volatile = analogReadMilliVolts(m_powerPin);
    if(abs(_volatile-m_volatileLast)>20)
    {
        m_volatileLast=_volatile;
    }
    return (m_volatileLast*2.0)/(m_batteryMaxVol*1000)*100;
}

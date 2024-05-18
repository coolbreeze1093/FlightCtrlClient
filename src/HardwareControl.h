
#include <ESP32Servo.h>
#include <ESP32PWM.h>

class HardwareControl
{
private:
    Servo m_servo1; // 创建舵机对象
    Servo m_servo2; // 创建舵机对象
    Servo m_servo3; // 创建舵机对象
    ESP32PWM m_pwm;
    ESP32PWM m_pwm2;

public:
    void init();
    HardwareControl(/* args */);
    ~HardwareControl();
    void servoCtrl_1(int angle);
    void servoCtrl_2(int angle);
    void servoCtrl_3(int angle);
    void pwmCtrl(float value);
    void pwm2Ctrl(float value);

};

void HardwareControl::init()
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    m_pwm.attachPin(13, 100, 10);
    m_pwm2.attachPin(14, 100, 10);
    m_servo1.attach(15);
    m_servo2.attach(4);
    m_servo3.attach(2);
}

HardwareControl::HardwareControl(/* args */)
{
}

HardwareControl::~HardwareControl()
{
}

void HardwareControl::servoCtrl_1(int angle)
{
    m_servo1.write(angle);
}

void HardwareControl::servoCtrl_2(int angle)
{
    m_servo2.write(angle);
}

void HardwareControl::servoCtrl_3(int angle)
{
    m_servo3.write(angle);
}

void HardwareControl::pwmCtrl(float value)
{
    uint32_t _value = map(value, 0, 100, 102, 230);

    //Serial.println(value);
    //Serial.println(_value);
    m_pwm.write(_value);
}

void HardwareControl::pwm2Ctrl(float value)
{
    uint32_t _value = map(value, 0, 100, 102, 230);

    //Serial.println(value);
    //Serial.println(_value);
    m_pwm2.write(_value);
}

#pragma once
#include "SbcInterface.h"
#include "ArduinoJson.h"
#include "HardwareControl.h"
#include "EspNewMan.h"
#include "PowerCalculation.h"

class MsgProcess : public SbcInterface
{
private:
    /* data */
    HardwareControl m_hardCtrl;
    bool m_isOpen = false;
    bool m_isInit = false;
    bool m_isClose = false;
    double m_initChannel1 = 0;
    double m_initChannel2 = 0;

    double m_curChannel1 = 0;
    double m_curChannel2 = 0;
    double m_curChannel3 = 0;
    double m_curChannel4 = 0;
    double m_curChannel5 = 0;

    clock_t m_internal;

    PowerCalculation *m_powerCal = nullptr;

public:
    MsgProcess(PowerCalculation *pc);
    ~MsgProcess();
    virtual void OnDataSent(esp_now_send_status_t status) override final;
    virtual void OnDataRecv(const uint8_t *data, int data_len) override final;
    virtual void run() override final;
    void HardWareCtrl();
};

MsgProcess::MsgProcess(PowerCalculation *pc) : m_powerCal(pc)
{
    m_internal = clock();
    m_hardCtrl.init();
}

MsgProcess::~MsgProcess()
{
}

void MsgProcess::OnDataSent(esp_now_send_status_t status)
{
    if (status != ESP_NOW_SEND_SUCCESS)
    {
        Serial.println("Delivery Fail");
    }
}

void MsgProcess::OnDataRecv(const uint8_t *data, int data_len)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // Test if parsing succeeds
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    if (doc.containsKey("HardCtrl"))
    {
        JsonObject _obj = doc["HardCtrl"].as<JsonObject>();
        m_curChannel1 = _obj["channel1"].as<double>();
        m_curChannel2 = _obj["channel2"].as<double>();
        m_curChannel3 = _obj["channel3"].as<double>();
        m_curChannel4 = _obj["channel4"].as<double>();
    }
    else if (doc.containsKey("volatile"))
    {
        JsonDocument doc;
        doc["volatile"] = m_powerCal->getRemainPower();
        String json;
        serializeJson(doc, json);
        EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
        m_internal=clock();
    }

    HardWareCtrl();
}

void MsgProcess::HardWareCtrl()
{
    m_hardCtrl.pwmCtrl(m_curChannel1);
    m_hardCtrl.pwm2Ctrl(m_curChannel2);
    m_hardCtrl.servoCtrl_1(m_curChannel3);
    m_hardCtrl.servoCtrl_2(m_curChannel4);
    m_hardCtrl.servoCtrl_3(m_curChannel5);
}

void MsgProcess::run()
{
    if (clock() - m_internal > 50000)
    {
        m_hardCtrl.pwmCtrl(m_initChannel1);
        m_hardCtrl.pwm2Ctrl(m_initChannel2);
        Serial.println("send vol");
        m_internal = clock();
    }
}
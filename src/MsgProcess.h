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
    double m_initChannel2 = 90;
    double m_initChannel3 = 90;
    double m_initChannel4 = 90;

    double m_curChannel1 = 0;
    double m_curChannel2 = 0;
    double m_curChannel3 = 0;
    double m_curChannel4 = 0;

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

MsgProcess::MsgProcess(PowerCalculation *pc):m_powerCal(pc)
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
    // String _str(data, data_len);
    // Serial.print(F("Recive Value: "));
    // Serial.println(_str);
    JsonDocument doc, _replyDoc;
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

        _replyDoc["HardCtrl"] = 1;
    }
    else if (doc.containsKey("InitValue"))
    {
        m_isInit = true;
        JsonObject _obj = doc["InitValue"].as<JsonObject>();
        m_initChannel1 = _obj["channel1"].as<double>();
        m_initChannel2 = _obj["channel2"].as<double>();
        m_initChannel3 = _obj["channel3"].as<double>();
        m_initChannel4 = _obj["channel4"].as<double>();
        _replyDoc["InitValue"] = 1;
    }
    else if (doc.containsKey("open"))
    {
        m_isOpen = true;
        _replyDoc["open"] = 1;
    }
    else if (doc.containsKey("close"))
    {
        m_isOpen = false;
        m_isClose = true;
        _replyDoc["close"] = 1;
    }
    else if (doc.containsKey("ping"))
    {
        _replyDoc["pong"] = 1;
    }
    else if (doc.containsKey("HeartBeat"))
    {
        m_internal = clock();
        _replyDoc["HeartBeat"] = 1;
    }
    String json;
    serializeJson(_replyDoc, json);
    EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
    // Serial.println(m_curChannel1);
    // Serial.println(m_curChannel2);
    // Serial.println(m_curChannel3);
    // Serial.println(m_curChannel4);
    HardWareCtrl();
}

void MsgProcess::HardWareCtrl()
{

    if (m_isOpen)
    {

        m_hardCtrl.pwmCtrl(m_curChannel1);
        m_hardCtrl.servoCtrl_1(m_curChannel2);
        m_hardCtrl.servoCtrl_2(m_curChannel3);
        m_hardCtrl.servoCtrl_3(m_curChannel4);
    }
    else if (m_isClose)
    {
        m_hardCtrl.pwmCtrl(m_initChannel1);
        m_hardCtrl.servoCtrl_1(m_initChannel2);
        m_hardCtrl.servoCtrl_2(m_initChannel3);
        m_hardCtrl.servoCtrl_3(m_initChannel4);
        m_isClose = false;
    }
    else if (m_isInit)
    {
        m_hardCtrl.pwmCtrl(m_initChannel1);
        m_hardCtrl.servoCtrl_1(m_initChannel2);
        m_hardCtrl.servoCtrl_2(m_initChannel3);
        m_hardCtrl.servoCtrl_3(m_initChannel4);
        m_isInit = false;
    }
}

void MsgProcess::run()
{
    if (clock() - m_internal > 5000)
    {
        m_hardCtrl.pwmCtrl(m_initChannel1);
        m_hardCtrl.servoCtrl_1(m_initChannel2);
        m_hardCtrl.servoCtrl_2(m_initChannel3);
        m_hardCtrl.servoCtrl_3(m_initChannel4);
        return;
    }
    JsonDocument doc;
    // Serial.println(_volatile);
    doc["volatile"] = m_powerCal->getRemainPower();
    String json;
    serializeJson(doc, json);
    EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
}
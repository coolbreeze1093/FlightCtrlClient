#include "EspNewMan.h"

EspNewMan EspNewMan::m_self;
SbcInterface *EspNewMan::m_sbc = nullptr;
EspNewMan::EspNewMan(/* args */)
{
    
}

EspNewMan::~EspNewMan()
{
}

bool EspNewMan::init()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.setTxPower(wifi_power_t::WIFI_POWER_19_5dBm);
    //  初始化ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        ESP.restart();
        return false;
    }

    // 注册回调函数
    if (esp_now_register_send_cb(EspNewMan::OnDataSent)!=ESP_OK)
    {
        Serial.println("Error esp_now_register_send_cb");
        return false;
    }
    if (esp_now_register_recv_cb(EspNewMan::OnDataRecv)!=ESP_OK)
    {
        Serial.println("Error esp_now_register_recv_cb");
        return false;
    }
    Serial.println("Success initializing ESP-NOW");
    Serial.println(WiFi.macAddress());
    return true;

}

bool EspNewMan::creatPeer(SbcInterface *sbc)
{
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, sbc->getMacAddr(), 6);
    peerInfo.channel = sbc->getChannel(); // 通道
    peerInfo.encrypt = sbc->getEncrypt(); // 是否加密为False
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return false;
    }
    m_sbc = sbc;
    return true;
}

bool EspNewMan::removePeer(SbcInterface *sbc)
{
    if (esp_now_del_peer(sbc->getMacAddr()) != ESP_OK)
    {
        Serial.println("Failed to remove peer");
        return false;
    }
    m_sbc = nullptr;
    return true;
}

bool EspNewMan::send(const uint8_t *mac_addr, const uint8_t *data, int size)
{
    // 发送信息到指定ESP32上

    esp_err_t result = esp_now_send(mac_addr, data, size);

    // 判断是否发送成功
    if (result == ESP_OK)
    {
        return true;
    }
    else
    {
        Serial.println("Error sending the data");
    }
    return false;
}

EspNewMan &EspNewMan::instance()
{
    return m_self;
}
void EspNewMan::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (m_sbc)
    {
        m_sbc->OnDataSent(status);
    }
}

void EspNewMan::OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    if (m_sbc)
    {
        m_sbc->OnDataRecv(data, data_len);
    }
}

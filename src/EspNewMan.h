#pragma once
#include <unordered_map>
#include "SbcInterface.h"
#include <esp_now.h>
#include <WiFi.h>

class EspNewMan
{
private:
    /* data */
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

    EspNewMan(/* args */);
    ~EspNewMan();

    static EspNewMan m_self;

    static SbcInterface*m_sbc;

    uint8_t *m_selfAddress=nullptr;
public:
    bool init();

    bool creatPeer(SbcInterface*sbc);

    bool removePeer(SbcInterface*sbc);

    bool send(const uint8_t*mac_addr, const uint8_t*data,int size);

    static EspNewMan& instance();

};
#define EspNewMan_H EspNewMan::instance()


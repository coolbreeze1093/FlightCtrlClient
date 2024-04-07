#pragma once
#include <esp_now.h>
#include <WiFi.h>
class SbcInterface
{
private:
    uint8_t *m_macAddr=nullptr;
    int m_channel=-1;
    bool m_encrypt=false;
public:
    SbcInterface():m_macAddr(new uint8_t[6]){};
    virtual ~SbcInterface(){};
    virtual void OnDataSent(esp_now_send_status_t status)=0;
    virtual void OnDataRecv(const uint8_t *data, int data_len)=0;
    virtual void run()=0;
    uint8_t * getMacAddr(){return m_macAddr;}
    int getChannel(){return m_channel;}
    bool getEncrypt(){return m_encrypt;}
    void setMacAddr(const uint8_t *addr){memcpy(m_macAddr,addr,6);}
    void setChannel(int channel){m_channel = channel;}
    void setEncrypt(bool encrypt){m_encrypt=encrypt;}
    

};


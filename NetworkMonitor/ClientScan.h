#ifndef ClientScan_h
#define ClientScan_h

#define maxClientScanResults 40

#include <ESP8266WiFi.h>
#include "Mac.h"
#include "MacList.h"

extern "C" {
#include "user_interface.h"
}

extern String data_getVendor(uint8_t first, uint8_t second, uint8_t third);

class ClientScan {
  public:
    ClientScan();
    void clearList();
    void packetSniffer(uint8_t *buf, uint16_t len, Mac ap);
    int add(Mac adr);
    bool sniffing = false;
    MacList clients;
    int results = 0;
    String sendResults();
    
    private:
      Mac from;
      Mac to;
      String getClientVendor(int num);
      Mac broadcast;
      Mac zero;
      int packets[maxClientScanResults];
      char vendors[maxClientScanResults][9];      
};


#endif

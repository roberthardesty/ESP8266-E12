#ifndef ClientModel_h
#define ClientModel_h

#include <Arduino.h>
#include "Mac.h"
#include "MacList.h"

extern "C" {
#include "user_interface.h"
}

class ClientModel {
  public: 
    ClientModel();
    Mac mac;
    int totalPackets;
    char* vendorLabel;
    bool isActive;
    int currentPacketRate();
    
  private: 
    
};

#endif

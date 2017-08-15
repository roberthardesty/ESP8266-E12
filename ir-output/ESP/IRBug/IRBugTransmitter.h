#ifndef IRBugTransmitter_h
#define IRBugTransmitter_h 
#include <IRsend.h>

const uint64_t VOLUME_MUTE  = 123456789; //hex code

bool send_single_ir_code(uint64_t code){  
  IRsend irsend(4);  // an IR led is connected to GPIO pin 4 (D2)
  irsend.begin();
  irsend.sendNEC(code);
}

bool send_ir_codes(String codeString){
  IRsend irsend(4);  // an IR led is connected to GPIO pin 4 (D2)
  irsend.begin();
  
  int i;
  for(i=0; i < codeString.length(); i++){
    uint64_t code = 0;//myHashOrDict[codeString[i]];  
    irsend.sendNEC(code);
    delay(3000);
  }
  return true;
}


#endif

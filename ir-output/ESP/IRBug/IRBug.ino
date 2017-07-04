
#include <IRsend.h>

IRsend irsend(4);  //GPIO pin 4 (D2)

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }

  //load config()
  //check for ghost mode()
  //wifi setup()
  //http request()
  //json parse()
  //spiffs save()
  //calculate sleep time()
  //finish
}

void loop() {
  /*
  Serial.println("NEC");
  irsend.sendNEC(0x00FFE01FUL, 32);
  delay(2000);
  */
  //TRANSLATE AND SEND Command String
}

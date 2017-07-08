
#include "IRBugConfig.h"
#include "IRBugTransmitter.h"

const unsigned long setupLength = 120000;
const unsigned long httpDelayLength = 10000;

IRBugConfig configFile;

void runSetupCycle(){
  bool isStillSetup = true;
  unsigned long startTime = millis();
  while(isStillSetup){
    send_single_ir_code(VOLUME_MUTE);
    unsigned long runTime = (millis() - startTime);
    isStillSetup = (runTime < setupLength);
    if(runTime % httpDelayLength == 0) bool foo = 1;//httpComm.setupGetRequest(); 
    delay(3000);
  }
}

void runGhostCycle(){
  ////var time = httpComm.localTimeGetRequest(); 
  ////String response httpComm.controllerGetRequest(); 
  send_ir_codes(configFile.ghostCode);
}

bool startWifi(){
  int attemptCount = 0
  WiFi.mode(WIFI_STA);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin("davidjohnson", "ytsedrah");
    Serial.print("Reconnecting...");
    attemptCount++;
  }
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(WiFi.status());
    WiFi.begin("davidjohnson", "ytsedrah");
    if(attemptCount > 12) return false;//try 13 times then report failure
    attemptCount++;
  }
  return true;//if we exit the loop we connected successfully
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }
  if(configFile.init() && configFile.wifiFailCount < 6){  
    //attempt to start wifi and prepare to save results at later time
    if(startWifi()){
      configFile.wifiFailCount = 0;
    }else{
      configFile.wifiFailCount + 1;
      Serial.print("Wifi Failed To Connect. Fail Count: " + configFile.wifiFailCount);
      //deepSleep(30000);//sleep for like 5 minutes
    }
    //check for special case modes
    if(configFile.isSetupMode) runSetupCycle();
    if(configFile.isGhost) runGhostCycle();
  }else{
    Serial.print("Either File Failed To Load Or Wifi Has Failed 6 times;
    //configFile.wifiFileCount = 0;//reset the fail count
    //configFile.save();
    //deepSleep(900000);//sleep for 1.5 hours or so
  }
  //*load config()
  //*check for ghost mode(); run();
  //wifi setup()
  //http request()
  //json parse()
  //battery voltage code
  //*spiffs save()
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

/**
 *  
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <EEPROM.h>
#include "data.h"
#include "ClientScan.h"
#include "Mac.h"
#include "MacList.h"
#include "ClientModel.h"
#include "ClientModelList.h"
#include "HttpComm.h"
#include "Settings.h"

extern "C" {
#include "user_interface.h"
}
#define USE_SERIAL Serial

HttpComm myHttp;
ClientScan clientScan;
Mac targetAP;
String targetAPSSID = "";

void sniffer(uint8_t *buf, uint16_t len) {
  clientScan.packetSniffer(buf, len, targetAP);
}

void startWifi(){
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(sniffer);
  WiFi.mode(WIFI_STA);
  WiFi.config(HOME_IP, HOME_GATEWAY, HOME_SUBNET);
  WiFi.begin(HOME_SSID, HOME_PASS);
  USE_SERIAL.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      USE_SERIAL.print((String)WiFi.status());
      WiFi.begin("davidjohnson", "ytsedrah");
    }
}

void stopWifi(){
  USE_SERIAL.println("Disconnecting from WiFi\n");
  USE_SERIAL.println("-----------------------------------------------\n");
  WiFi.disconnect();
  wifi_set_opmode(STATION_MODE);
}

void findMyNetwork(String mySsid){
  while(targetAPSSID == ""){    
    int results = WiFi.scanNetworks(false, true);
    for(int i = 0; i < results; i++){
      USE_SERIAL.print(".");
      if(WiFi.SSID(i) == mySsid){
        USE_SERIAL.print("\n\nTarget network " + WiFi.SSID(i)+ " has been located\n");
        USE_SERIAL.println("-----------------------------------------------\n");
        targetAPSSID = WiFi.SSID(i);
        targetAP.set(WiFi.BSSID(i)[0], WiFi.BSSID(i)[1], WiFi.BSSID(i)[2], WiFi.BSSID(i)[3], WiFi.BSSID(i)[4], WiFi.BSSID(i)[5]);
        return;
      }      
    }
    delay(2000);
  }
}

void httpPostResults(){
  String foundClientsJSON = clientScan.sendResults();
  USE_SERIAL.println("[HTTP] attempting to POST " + foundClientsJSON + "\n");
  USE_SERIAL.println("Connected: " + (String)WiFi.status());
  myHttp.defaultPOST(foundClientsJSON);
}

void setup() {
  USE_SERIAL.begin(115200);
  USE_SERIAL.println(); USE_SERIAL.println(); USE_SERIAL.println();
  for(uint8_t t = 3; t > 1; t--) {
      USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  int startTime = millis();
  
  EEPROM.begin(4096);
  SPIFFS.begin();
  USE_SERIAL.print("Finding My Network\n\n");
  findMyNetwork(HOME_SSID);
  startWifi();
  USE_SERIAL.println("Time from startup to connection: " + (String)(millis()-startTime));
}

static bool isPostMode = false;
long roundInterval = 3000;
unsigned long previousRoundTime = 0;
static int testCount = 0;

void loop() {
  unsigned long currentRoundTime = millis();
  if(currentRoundTime - previousRoundTime > roundInterval){
    previousRoundTime = currentRoundTime;
    USE_SERIAL.println("Time Round: " + (String)testCount);
    
    stopWifi();
    startWifi();
    isPostMode = true;
    USE_SERIAL.println("-----------------------------------------------\n");
    USE_SERIAL.print("Done. prepping to post..");
    USE_SERIAL.println("-----------------------------------------------\n");
    testCount++; 
  }
 
  if(isPostMode){
    httpPostResults();
    USE_SERIAL.println("\nStarting Network Monitor...\n");
    wifi_promiscuous_enable(1);
    isPostMode = false;
    clientScan.clearList();
  }
  
}


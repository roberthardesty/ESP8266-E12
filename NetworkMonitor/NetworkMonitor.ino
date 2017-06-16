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
  WiFi.config(HOME_STATIC_IP, HOME_STATIC_GATEWAY, HOME_STATIC_SUBNET);
  WiFi.begin(HOME_SSID, HOME_PASS);
  USE_SERIAL.print("Reconnecting...");
      while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      USE_SERIAL.print(WiFi.status());
      WiFi.begin("davidjohnson", "ytsedrah");
    }
}

void stopWifi(){
  USE_SERIAL.println("Disable promis\n");  
  wifi_promiscuous_enable(0);
  USE_SERIAL.println("stopping WiFi\n");
  USE_SERIAL.println("-----------------------------------------------\n");
  WiFi.disconnect();
  wifi_set_opmode(STATION_MODE);
  //wifi_set_channel(WiFi.channel());
 // wifi_promiscuous_enable(1);
  //USE_SERIAL.println("enable promis");  
}

void findMyNetwork(String mySsid){
  while(targetAPSSID == ""){
    USE_SERIAL.print("Finding My Network.");
    
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
    delay(3000);
  }
}

void httpPostResults(){
  String foundClientsJSON = clientScan.sendResults();
  USE_SERIAL.print("[HTTP] attempting to POST " + foundClientsJSON + "\n");
  myHttp.defaultPOST(foundClientsJSON);
}

void setup() {
  USE_SERIAL.begin(115200);
 // USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.println(); USE_SERIAL.println(); USE_SERIAL.println();
  for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  EEPROM.begin(4096);
  SPIFFS.begin();
  findMyNetwork("davidjohnson");
  startWifi();
  myHttp.defaultPOST("test");
}
bool isSniffingMode = false;
bool isPostMode = false;
int count = 0;
long roundTimeInterval = 15000;
unsigned long currentRoundTime;
unsigned long previousRoundTime = 0;
void loop() {
  
  currentRoundTime = millis();
  if(currentRoundTime - previousRoundTime > roundTimeInterval){
    if(clientScan.checkNetwork()) prepForPost();

    previousRoundTime = currentRoundTime;
  }
  if(isPostMode){
    httpPostResults();
    USE_SERIAL.println("\nStarting Network Monitor...\n");
    wifi_promiscuous_enable(1);
    isPostMode = false;
  } 
}

void prepForPost(){
  stopWifi();
  startWifi();
  isPostMode = true;
  USE_SERIAL.println("-----------------------------------------------\n");
  USE_SERIAL.print("\n\nRound: #" + (String)(count) + " Done. prepping to post..");
  USE_SERIAL.println("-----------------------------------------------\n"); 
  count++;
}


/**
 *  
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <EEPROM.h>
#include "data.h"
#include "ClientScan.h"
#include "Mac.h"
#include "MacList.h"

extern "C" {
#include "user_interface.h"
}

#define USE_SERIAL Serial
HTTPClient http; 
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
  WiFi.begin("davidjohnson", "ytsedrah");
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
  USE_SERIAL.print("[HTTP] begin...\n");
  http.begin("192.168.1.68", 1880, "/test", false); //HTTP
  http.addHeader("Content-Type", "text/plain");
  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  String foundClientsJSON = clientScan.sendResults();
  int httpCode = http.POST(foundClientsJSON);
  
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
  
      // file found at server
      if(httpCode == 200) {
          String payload = http.getString();
          USE_SERIAL.println("Sucessfully delivered: " + payload);
          USE_SERIAL.println("-----------------------------------------------\n");
      }
  } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: ");
  }
  
  http.end();
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
}
bool isSniffingMode = false;
bool isPostMode = false;
int count = 0;

void loop() {
  if(count == 1) {
    USE_SERIAL.println("\nStarting Network Monitor...\n");
    wifi_promiscuous_enable(1);
  }
  if(count % 4 == 0) {
    stopWifi();
    startWifi();
    isPostMode = true;
    USE_SERIAL.println("-----------------------------------------------\n");
    USE_SERIAL.print("\n\nRound: #" + (String)(count/4) + " Done. prepping to post..");
    USE_SERIAL.println("-----------------------------------------------\n");
  }
  
 
  if(isPostMode){
    httpPostResults();
    USE_SERIAL.println("\nStarting Network Monitor...\n");
    wifi_promiscuous_enable(1);
    isPostMode = false;
  }
  
  delay(4000);
  count++;
}


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
  USE_SERIAL.println("||||");
}

void startWifi(){
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(sniffer);
  WiFi.mode(WIFI_STA);
  WiFi.begin("davidjohnson", "ytsedrah");
  USE_SERIAL.print("Reconnecting...\n");
  delay(2000);
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

void httpPostResults(int i){
  USE_SERIAL.print("[HTTP] begin...\n");
  http.begin("192.168.1.68", 1880, "/test", false); //HTTP
  http.addHeader("Content-Type", "text/plain");
  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  String foundClientsJSON = clientScan.sendResults(i);
  int httpCode = http.POST(foundClientsJSON);
  
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
  
      // file found at server
      if(httpCode == 200) {
          String payload = http.getString();
          USE_SERIAL.println(payload);
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
  startWifi();
}
bool isScanningMode = false;
bool isSniffingMode = false;
bool isPostMode = false;
int count = 0;

void loop() {
  isScanningMode = targetAPSSID == "";
  USE_SERIAL.println("target ap mac: " + targetAP.toString());
  USE_SERIAL.print("target ssid: " + targetAPSSID + "\n");
  if(count == 0 || isScanningMode){
    USE_SERIAL.print("No target AP, enabling scan mode...\n");
    stopWifi();
    int results = WiFi.scanNetworks(false, true);
    for(int i = 0; i < results; i++){
      if(WiFi.SSID(i) == "davidjohnson"){
        USE_SERIAL.print("Target AP Found: " + WiFi.SSID(i)+ "\n");
        targetAPSSID = WiFi.SSID(i);
        targetAP.set(WiFi.BSSID(i)[0], WiFi.BSSID(i)[1], WiFi.BSSID(i)[2], WiFi.BSSID(i)[3], WiFi.BSSID(i)[4], WiFi.BSSID(i)[5]);
      }      
    }
    startWifi();
  }
  if(count == 3) wifi_promiscuous_enable(1);
  if(count == 5) {
    startWifi();
    String deviceList = "";
    for(int i=0; i < 10; i++){
      deviceList += clientScan.clients._get(i).toString();
    }
    isPostMode = true;
    USE_SERIAL.println("Done. prepping to post....");
  }
  delay(6000);
  if(isPostMode){
  for (int i = 0; i < clientScan.results; i++){
    httpPostResults(i);
    delay(1000);
  }
    
  }
  count++;
}


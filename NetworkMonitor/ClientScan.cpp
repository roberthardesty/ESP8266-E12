#include "ClientScan.h"


ClientScan::ClientScan() {
  broadcast.set(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
  zero.set(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
}

void ClientScan::clearList() {
  clients._clear();
  for (int i = 0; i < maxClientScanResults; i++) {
    packets[i] = 0;
  }
}

int ClientScan::add(Mac adr){
  int clientNum = clients.getNum(adr);
  if (clientNum == -1) {
    Serial.println("Adding: " + adr.toString());
    data_getVendor(adr._get(0), adr._get(1), adr._get(2)).toCharArray(vendors[results], 9);
    results++;
    int clientNum = clients.add(adr);
  }
  return clientNum;
}

String ClientScan::sendResults(){
  String json = "{\"clients\":[";
  for(int i = 0; i < results; i++){
    json += "{";
    json += "\"i\":" + (String)i + ",";
    json += "\"packets\":" + (String)packets[i] + ",";
    json += "\"mac\":\"" + clients._get(i).toString() + "\",";
    json += "\"vendor\":\"" + (String)getClientVendor(i) + "\"";
    json += "}";
    if ((i != results - 1) && (i != maxClientScanResults - 1)) json += ",";  
    else json += "]";  
  }     
  return json;
}

String ClientScan::getClientVendor(int num) {
  return vendors[num];
}

int prevPackets = 0;
char chromecast[9] = {"Azurewav"};
bool ClientScan::checkNetwork(){
  int newPackets;
  int i;
  for(i=0; i < results; i++)
  {
      Serial.println(vendors[i]);
      if((String)vendors[i] == (String)chromecast)
      {
        newPackets = packets[i] - prevPackets;
        prevPackets += newPackets;
        Serial.println("\n\nNew Packets from Azurewav: " + (String)newPackets);
      }
  }
  
  return newPackets > 2000;
}

void ClientScan::packetSniffer(uint8_t *buf, uint16_t len, Mac ap) {
  if (len > 27) {
    from.set(buf[16], buf[17], buf[18], buf[19], buf[20], buf[21]);
    to.set(buf[22], buf[23], buf[24], buf[25], buf[26], buf[27]);
    if (ap.compare(from)) {
      int clientNum = add(to);
      packets[clientNum]++;
    }
  }
}



#include "ClientModelList.h"

ClientModelList::ClientModelList() {

}

bool ClientModelList::contains(Mac adr) {
  for (int i = 0; i < num; i++) {
    if (clientModels[i].mac.compare(adr)) return true;
  }
  return false;
}

int ClientModelList::add(Mac adr, char vendor[9]) {
  if(num < len && adr.valid()) {
    if (!contains(adr)) {
      clientModels[num].mac.setMac(adr);
      clientModels[num].vendorLabel = vendor;
      clientModels[num].totalPackets = 0;
      clientModels[num].isActive = true;
      num++;
      return num - 1;
    }
  }
  return -1;
}

ClientModel ClientModelList::_get(int i) {
  return clientModels[i];
}

void ClientModelList::addPacket(int i){
  clientModels[i].totalPackets++;
}

int ClientModelList::getNum(Mac cmMac) {
  for (int i = 0; i < num; i++) {
    if (clientModels[i].mac.compare(cmMac)) {
      return i;
    }
  }
  return -1;
}

void ClientModelList::_clear() {
  for (int i = 0; i < num; i++) {
    clientModels[i].mac.set(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    clientModels[i].totalPackets = 0;
    clientModels[i].isActive = false;
  }
  num = 0;
}

void ClientModelList::remove(ClientModel cm) {
  for (int i = 0; i < num; i++) {
    if (clientModels[i].mac.compare(cm.mac)) {
      clientModels[i].mac.set(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      clientModels[i].totalPackets = 0;
      clientModels[i].isActive = false;
    }
  }
}

void ClientModelList::set(int num, ClientModel cm) {
  clientModels[num] = cm;
}


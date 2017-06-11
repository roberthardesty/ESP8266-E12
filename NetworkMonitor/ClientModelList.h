#ifndef ClientModelList_h
#define ClientModelList_h

#define listLen 20

#include "ClientModel.h"
#include "Mac.h"

class ClientModelList
{
  public:
    ClientModelList();
    int add(Mac adr, char vendor[9]);
    void remove(ClientModel cm);
    void set(int num, ClientModel cm);
    bool contains(Mac adr);
    int getNum(Mac adr);
    ClientModel _get(int i);
    void addPacket(int clientNum);
    void _clear();

    int num = 0;
    ClientModel clientModels[listLen];
  private:
    int len = listLen;
    void addPacket(Mac adr);
};

#endif


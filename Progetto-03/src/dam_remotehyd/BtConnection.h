#ifndef __BTCONNECTION__
#define __BTCONNECTION__
#include "SoftwareSerial.h"


class BtConnection {

  public:
    BtConnection();
    void sendStatus(String str);
    String receiveStatus();
};

#endif

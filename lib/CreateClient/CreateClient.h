#ifndef CreateClient_h
#define CreateClient_h
#include <BLEDevice.h>
#include <Arduino.h>

class CreateClient : public BLEClientCallbacks {  //Hereda de BLEClientCallbacks

    public:
        void onConnect(BLEClient *pClient);
        void onDisconnect(BLEClient *pClient);
        void createBleClientWithCallbacks();
};

#endif


#include <Arduino.h>
#include "ConnectDevice.h"

bool connectionSuccessful = false;
extern BLEClient* pClient;

void ConnectDevice::ConnectSensor() {
    if (pClient == nullptr) {
        Serial.println("Error: BLE client not initialized!");
        return;
    }
    
    if (pClient->connect(BLEAddress(_address))) {
        connectionSuccessful = true;
        Serial.println(" -Connected to sensor \n");
    } else {
        Serial.println(" -Failed to connect to sensor \n");
        pClient->disconnect();
    }
};


#include <Arduino.h>
#include "CreateClient.h"

extern BLEClient* pClient;

void CreateClient::onConnect(BLEClient* pClient) {
    Serial.println(" -Client Connected \n");
}

void CreateClient::onDisconnect(BLEClient* pClient) {
    Serial.println(" -Client Disconnected \n");
}

void CreateClient::createBleClientWithCallbacks() {
    BLEDevice::init("ESP32");

    pClient = BLEDevice::createClient();
    pClient -> setClientCallbacks(new CreateClient());
    Serial.println(" -Client Created \n");
};


#include <Arduino.h>
#include "Comm.h"

extern BLEClient* pClient;

float Comm::lastTemperature = 0.0f;
float Comm::lastHumidity = 0.0f;
bool Comm::newTemperatureReceived = false;
bool Comm::newHumidityReceived = false;
bool Comm::dataReceived = false;
const char* Comm::sensorId = nullptr;

BLEUUID Comm::serviceUUID;
BLEUUID Comm::tempCharUUID;
BLEUUID Comm::humCharUUID;

Comm::Comm(const char* UUID_Service, const char* UUID_TempCharacteristic, const char* UUID_HumCharacteristic)
    : _UUID_Service(UUID_Service), _UUID_TempCharacteristic(UUID_TempCharacteristic), _UUID_HumCharacteristic(UUID_HumCharacteristic) {

    serviceUUID = BLEUUID(_UUID_Service); // The remote service we wish to connect to.
    tempCharUUID = BLEUUID(_UUID_TempCharacteristic); // The characteristic of the remote service we are interested in.
    humCharUUID = BLEUUID(_UUID_HumCharacteristic); // The characteristic of the remote service we are interested in.
    sensorId = UUID_Service;
}

void Comm::registerNotification() { // Method to register feature notification

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return;
    }
    Serial.println(" -Service Found");
    Serial.println(pRemoteService->getUUID().toString().c_str());

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    BLERemoteCharacteristic* pRemoteCharacteristicTemperature = pRemoteService->getCharacteristic(tempCharUUID);
    BLERemoteCharacteristic* pRemoteCharacteristicHumidity = pRemoteService->getCharacteristic(humCharUUID);
    Serial.println(" -Features Found");
    Serial.println(pRemoteCharacteristicTemperature->getUUID().toString().c_str());
    Serial.println(pRemoteCharacteristicHumidity->getUUID().toString().c_str());
    Serial.println("\n");

    pRemoteCharacteristicTemperature->registerForNotify(notifyCallback);
    pRemoteCharacteristicHumidity->registerForNotify(notifyCallback);
}

void Comm::notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    
    byte buffer[2];

    if (pBLERemoteCharacteristic->getUUID().equals(tempCharUUID)) {
        buffer[0] = pData[0];
        buffer[1] = pData[1];
        lastTemperature = (buffer[0] | (buffer[1] << 8)) * 0.1;
        newTemperatureReceived = true; // New temperature reading
    } else if (pBLERemoteCharacteristic->getUUID().equals(humCharUUID)) {
        buffer[0] = pData[0];
        buffer[1] = pData[1];
        lastHumidity = (buffer[0] | (buffer[1] << 8)) * 0.01;
        newHumidityReceived = true; // New humidity reading
    } else {
        Serial.println("Unknown characteristic");
    }

    if (newTemperatureReceived && newHumidityReceived) {
        dataReceived = true;
    }
}


void Comm::printTemperatureAndHumidity() { // Método para imprimir las lecturas de temperatura y humedad

    Serial.printf("Temperature = %.1f °C : Humidity = %.1f %%\n", lastTemperature, lastHumidity);
    Serial.println("");

    newTemperatureReceived = false;
    newHumidityReceived = false;
};



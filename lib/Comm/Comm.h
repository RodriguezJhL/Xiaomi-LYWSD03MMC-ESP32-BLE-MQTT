#ifndef Comm_h
#define Comm_h

#include <BLEDevice.h>
#include <Arduino.h>
#include "ConnectDevice.h"

class Comm {
    public:
        Comm(const char* UUID_Service, const char* UUID_TempCharacteristic, const char* UUID_HumCharacteristic);
        void registerNotification();
        static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
        static void printTemperatureAndHumidity();
        static float lastTemperature;
        static float lastHumidity;
        static bool newTemperatureReceived;
        static bool newHumidityReceived;
        static bool dataReceived;
        static const char* sensorId;
    private:
        const char* _UUID_Service;
        const char* _UUID_TempCharacteristic;
        const char* _UUID_HumCharacteristic;
        static BLEUUID serviceUUID;
        static BLEUUID tempCharUUID;
        static BLEUUID humCharUUID;
};

#endif

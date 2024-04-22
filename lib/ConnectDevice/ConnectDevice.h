#ifndef ConnectDevice_h
#define ConnectDevice_h
#include <BLEDevice.h>
#include <Arduino.h>

class ConnectDevice {
    public:
        ConnectDevice(const char* address){
            this->_address = address;
        };
        void ConnectSensor();

    private:
        const char* _address;
};

#endif
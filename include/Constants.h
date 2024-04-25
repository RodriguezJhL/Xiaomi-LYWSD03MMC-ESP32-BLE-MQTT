#ifndef Constants_h
#define Constants_h

#include <Arduino.h>

namespace Constants {

    //Datos del Sensor
    const char* MAC = "A4:C1:38:B6:99:B2";
    const char* ServiceUUID = "0000181a-0000-1000-8000-00805f9b34fb";
    const char* CharTempUUID = "00002a1f-0000-1000-8000-00805f9b34fb";
    const char* CharHumUUID = "00002a6f-0000-1000-8000-00805f9b34fb";

    //Datos del Broker MQTT
    const char broker[] = "broker.hivemq.com";
    int port = 1883;
    const char* topic = "sda-iot-2024/xiaomi";

    //Datos del WiFi
    char ssid[] = "Samantha";
    char password[] = "1001686436";
}

#endif

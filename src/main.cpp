#include <Arduino.h>
#include <BLEDevice.h>
#include "Constants.h"
#include "CreateClient.h"
#include "ConnectDevice.h"
#include "Comm.h"

BLEClient* pClient;
CreateClient Micliente;

ConnectDevice Device = Constants::MAC; 
Comm Sensor(Constants::ServiceUUID, Constants::CharTempUUID, Constants::CharHumUUID); 

void setup() {
  Serial.begin(115200);

  Micliente.createBleClientWithCallbacks();
  Device.ConnectSensor();
  Sensor.registerNotification();
}

void loop() {
  if (Comm::dataReceived) {
    if (strcmp(Comm::sensorId, Constants::ServiceUUID) == 0) {
      Sensor.printTemperatureAndHumidity();
    } 

    Comm::dataReceived = false; // Reset the flag
  }
}
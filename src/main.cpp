#include <Arduino.h>
#include <BLEDevice.h>
#include "Constants.h"
#include "CreateClient.h"
#include "ConnectDevice.h"
#include "Comm.h"
#include <WiFi.h>
#include <PubSubClient.h>

BLEClient* pClient;
CreateClient Micliente;

WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

ConnectDevice Device = Constants::MAC; 
Comm Sensor(Constants::ServiceUUID, Constants::CharTempUUID, Constants::CharHumUUID); 

void ConnectWifi() {
  Serial.println("Attempting to connect to WPA SSID: ");
  Serial.println(Constants::ssid);
  WiFi.begin(Constants::ssid, Constants::password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(3000);
  }

  Serial.println("Connected to the network!");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  delay(5000);
}

void ConnectMQTT() {
  Serial.println("Attempting to connect to the MQTT broker: ");
  Serial.println(Constants::broker);
  mqttClient.setServer(Constants::broker, Constants::port);

  while (!mqttClient.connected()) {
    Serial.println("Waiting MQTT Connection... \n");
    if (mqttClient.connect("ESPMAIN")) {
      Serial.println("You're connected to the MQTT broker! \n");
    } 
  }
  delay(3000);
}

void setup() {
  Serial.begin(115200);
  ConnectWifi();
  ConnectMQTT();
  delay(3000);

  Micliente.createBleClientWithCallbacks();
  Device.ConnectSensor();
  Sensor.registerNotification();
}

void loop() {
  if (Comm::dataReceived) {
    if (strcmp(Comm::sensorId, Constants::ServiceUUID) == 0) {
      // Sensor.printTemperatureAndHumidity();
      Sensor.sendMQTT(Constants::topic);
    } 

    Serial.println("");
    Comm::dataReceived = false; // Reset the flag
  }
}
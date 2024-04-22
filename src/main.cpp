#include <Arduino.h>
#include <BLEDevice.h>

// Declara una variable para realizar un seguimiento de si se han recibido ambas lecturas
bool temperatureReceived = false;
bool humidityReceived = false;
byte buffer[3];

BLEClient* pClient;
#define LYWSD03MMC_ADDR "A4:C1:38:B6:99:B2"
static BLEAddress htSensorAddress(LYWSD03MMC_ADDR);
bool connectionSuccessful = false;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("0000181a-0000-1000-8000-00805f9b34fb");
// The characteristics of the remote service we are interested in.
static BLEUUID charUUID("00002a1f-0000-1000-8000-00805f9b34fb");
static BLEUUID humCharUUID("00002a6f-0000-1000-8000-00805f9b34fb");

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("Connected");
  }

  void onDisconnect(BLEClient* pclient) {
    Serial.println("Disconnected");
    if (!connectionSuccessful) {
      Serial.println("RESTART");
      ESP.restart();
    }
  }
};

void SerialFlush() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  float temp, humedad;
  SerialFlush();

  if (pBLERemoteCharacteristic->getUUID().equals(charUUID)) {
    buffer [0] = (pData[0]);
    buffer [1] = (pData[1]);
 
  } else if (pBLERemoteCharacteristic->getUUID().equals(humCharUUID)) {
    buffer [2] = (pData[0]);
    buffer [3] = (pData[1]);

  } else {
    Serial.println("Unknown characteristic");
  }
  temp = (buffer[0] | (buffer[1] << 8)) * 0.1;
  humedad = (buffer[2] | (buffer[3] << 8)) * 0.01;

  Serial.printf("Temperature = %.1f °C : Humidity = %.1f %%\n", temp, humedad);
}



void registerNotification() {  //connectToServe

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return;
  }
  Serial.println(" - Found our service");
  Serial.println(pRemoteService->getUUID().toString().c_str());

  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  BLERemoteCharacteristic* pRemoteCharacteristicHumedad = pRemoteService->getCharacteristic(humCharUUID);
  if (pRemoteCharacteristicHumedad == nullptr && pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find characteristics UUID: ");
    Serial.println(charUUID.toString().c_str());
    Serial.println(humCharUUID.toString().c_str());
    pClient->disconnect();
    return;
  }
  Serial.println(" - Found temp and humidity characteristics");
  Serial.println(pRemoteCharacteristic->getUUID().toString().c_str());
  Serial.println(pRemoteCharacteristicHumedad->getUUID().toString().c_str());

  pRemoteCharacteristic->registerForNotify(notifyCallback);
  pRemoteCharacteristicHumedad->registerForNotify(notifyCallback);
}

void createBleClientWithCallbacks() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor() {
  if (pClient->connect(htSensorAddress)) {
    connectionSuccessful = true;
    Serial.println("Connected to sensor");
  } else {
    Serial.println("Failed to connect to sensor");
    pClient->disconnect();
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting MJ client...");
  delay(500);

  BLEDevice::init("ESP32");
  createBleClientWithCallbacks();
  delay(500);
  connectSensor();
  registerNotification();
}

void loop() {
}

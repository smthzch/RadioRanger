/* 
===========================================================
            Radio Ranger

This device connects to an equivalent device via bluetooth,
reads the signal strength and updates a LED bar visualizing
the relative strength.
===========================================================
            Pin Layout

BATT(+/-)-|A0  USB  5V|-
         -|D1      GND|-LED(G)
         -|D2      3V3|-LED(VCC)
         -|D3      D10|-
LED(SDA) -|D4       D9|-
LED(SCL) -|D5       D8|-
         -|D6       D7|-
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "battery.h"
#include "ledbar.h"

// Define a simple service UUID (can be anything unique)
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLEServer* pServer = NULL;
bool deviceConnected = false;

Adafruit_24bargraph bar = Adafruit_24bargraph();

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Client connected!");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Client disconnected – restarting advertising");
      pServer->startAdvertising();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        int num_bars = std::stoi(value);
        setBar(bar, num_bars);
      }
    }
};


void setup() {
  pinMode(A0, INPUT); // ADC for battery voltage
  Serial.begin(115200);
  delay(1500);  // For XIAO USB CDC

  initBar(bar);

  // Initialize BLE
  BLEDevice::init("XIAO_Peripheral");  // Device name
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create a dummy service (required for advertising)
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Helps with iOS connections
  pAdvertising->setMinPreferred(0x12);
  pServer->getAdvertising()->start();

  // Print BLE MAC address for client to use
  Serial.print("BLE Address: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());
  Serial.println("Waiting for connection...");
}

void loop() {
  float Vbattf = batteryVoltage(A0);
  Serial.println(Vbattf, 3);
  delay(1000);
}

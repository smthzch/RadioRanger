/* 
===========================================================
            Radio Ranger

This device connects to an equivalent device via bluetooth,
reads the signal strength and updates a LED bar visualizing
the relative strength.
===========================================================
            Pin Layout

        -|A0  USB  5V|-
        -|D1      GND|-
        -|D2      3V3|-
        -|D3      D10|-
        -|D4       D9|-
        -|D5       D8|-
        -|D6       D7|-

There is no pin layout, connect a battery.
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>

const char* MY_DEVICE_NAME = "RADIO";


void setup() {
  Serial.begin(9600);
  delay(1500);  // For XIAO USB CDC

  // BLE init
  BLEDevice::init(MY_DEVICE_NAME);

  // Start advertising (peripheral mode)
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Helps with compatibility
  pAdvertising->start();
  Serial.println("Advertising started");
  
  // Print BLE MAC address for client to use
  Serial.print("BLE Address: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());
  Serial.println("Waiting for connection...");
  
}

void loop() {}

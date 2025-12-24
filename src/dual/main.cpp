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

#if defined(RADIO)
  const char* MY_DEVICE_NAME = "RADIO";
  static const char* OTHER_DEVICE_MAC = "94:a9:90:69:e6:12";
#elif defined(RANGER)
  const char* MY_DEVICE_NAME = "RANGER";
  static const char* OTHER_DEVICE_MAC = "94:a9:90:67:00:16";
#else
  #error "Please define RADIO or RANGER in build flags or at top of sketch"
#endif

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "Adafruit_LEDBackpack.h"
#include "battery.h"
#include "ledbar.h"

Adafruit_24bargraph  bar = Adafruit_24bargraph();

static BLEAddress targetAddr(OTHER_DEVICE_MAC);

BLEScan* pBLEScan = nullptr;
unsigned long previousMillis = 0;
const long interval = 500;  // Scan + update every 1.1 seconds
int no_contact = 1;
float min_rssi = -100;
float smooth_rssi = min_rssi;
float alpha = 0.2;


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(targetAddr)) {
      smooth_rssi = alpha * (float)advertisedDevice.getRSSI() + (1 - alpha)* smooth_rssi;
      no_contact = 0;
    }
  }
};

// Dummy callback for asynchronous scanning
void onScanComplete(BLEScanResults results) {}

void setup() {
  Wire.setClock(100000);
  initBar(bar);

  pinMode(A0, INPUT); // ADC for battery voltage
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
  
  // Setup scanner (central mode)
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(false);  // Passive scan saves power
  pBLEScan->setInterval(200);
  pBLEScan->setWindow(100);
  pBLEScan->start(0, &onScanComplete, false);
  Serial.println("Continuous scanning started");

  // Print BLE MAC address for client to use
  Serial.print("BLE Address: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());
  Serial.println("Waiting for connection...");
  
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    pBLEScan->stop();

    no_contact++;
    if (no_contact > 4){
      smooth_rssi = min_rssi;
    }

    // Serial output
    Serial.print("RSSI: ");
    Serial.print(smooth_rssi);
    Serial.println(" dBm");

    // LED output
    setBar(bar, smooth_rssi);

    float Vbattf = batteryVoltage(A0);
    Serial.println(Vbattf, 3);
    if (Vbattf > 3.6){
      bar.blinkRate(0);
    }else if(Vbattf > 3.4){
      bar.blinkRate(2);
    }else{
      bar.blinkRate(1);
    }

    pBLEScan->start(0, &onScanComplete, false);
  }
}

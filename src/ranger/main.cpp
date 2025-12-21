/* 
===========================================================
          Radio Ranger

This device connects to an equivalent device via bluetooth,
reads the signal strength and updates a LED bar visualizing
the relative strength.
===========================================================
          Pin Layout

BATT    -|A0  USB  5V|-
        -|D1      GND|-LED(G)
        -|D2      3V3|-LED(VCC)
        -|D3      D10|-
LED(SDA)-|D4       D9|-
LED(SCL)-|D5       D8|-
        -|D6       D7|-
*/

#include <BLEDevice.h>
#include <BLEClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "battery.h"
#include "ledbar.h"

static BLEAddress peripheralAddress("94:a9:90:67:00:16");
BLEClient* pClient = nullptr;
bool connected = false;

unsigned long previousMillis = 0;
const long interval = 500;  // Read RSSI every 0.5 seconds
float smooth_rssi = -100;
float alpha = 0.2; // how much new value to use
int max_db = -40;
int min_db = -100;

Adafruit_24bargraph bar = Adafruit_24bargraph();

void connectToPeripheral() {
  if (pClient->connect(peripheralAddress)) {
    connected = true;
    Serial.println("Signal LOCK");
    bar.blinkRate(0);
    delay(1000);
  } else {
    Serial.println("Failed to connect – retrying in 1s");

    delay(1000);
  }
}

void setup() {
  pinMode(A0, INPUT); // ADC for battery voltage
  
  Serial.begin(11520);
  delay(1500);
  Serial.println("Acquiring Signal");

  initBar(bar);

  BLEDevice::init("");
  pClient = BLEDevice::createClient();
  connectToPeripheral();
}



void loop() {
  if (!connected) {
    connectToPeripheral();
    return;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Get RSSI while connected
    float rssi = (float)pClient->getRssi();
    if (rssi != 0) {  // 0 means invalid
      smooth_rssi = alpha * rssi + (1 - alpha) * smooth_rssi;

      // Print to Serial
      Serial.print("RSSI: ");
      Serial.print(smooth_rssi);
      Serial.println(" dBm");

      // Asterisk bar (0-16 stars based on quality)
      int num_stars = map(constrain(smooth_rssi, min_db, max_db), min_db, max_db, 0, 24);
      setBar(bar, num_stars);
    } else {
      Serial.println("Invalid RSSI – check connection");
    }
  }

  // Check if still connected
  if (!pClient->isConnected()) {
    connected = false;
    Serial.println("Acquiring Signal");
  }
}

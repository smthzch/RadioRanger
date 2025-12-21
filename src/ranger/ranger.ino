/* 
===========================================================
          Radio Ranger

This device connects to an equivalent device via bluetooth,
reads the signal strength and updates a LED bar visualizing
the relative strength.
===========================================================
          Pin Layout

        -|D0  USB  5V|-
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

static BLEAddress peripheralAddress("94:a9:90:67:00:16");

Adafruit_24bargraph bar = Adafruit_24bargraph();


BLEClient* pClient = nullptr;
bool connected = false;
unsigned long previousMillis = 0;
const long interval = 500;  // Read RSSI every 0.5 seconds
float smooth_rssi = -100;
float alpha = 0.2; // how much new value to use

int max_db = -40;
int min_db = -100;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("Acquiring Signal");

  bar.begin(0x70);  // pass in the address

  for (uint8_t b=0; b<24; b++ ){
    if ((b % 3) == 0)  bar.setBar(b, LED_RED);
    if ((b % 3) == 1)  bar.setBar(b, LED_YELLOW);
    if ((b % 3) == 2)  bar.setBar(b, LED_GREEN);
  }
  bar.writeDisplay();
  bar.blinkRate(1);
  BLEDevice::init("");

  pClient = BLEDevice::createClient();
  connectToPeripheral();
}

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
      //Serial.print("Strength: ");
      for (int i=23; i>=0; i--) {
        //Serial.print(i);
        if ((i / 8) == 0)  bar.setBar(i, LED_GREEN);
        else if ((i / 16) == 0)  bar.setBar(i, LED_YELLOW);
        else if ((i / 24) == 0)  bar.setBar(i, LED_RED);
      }
      for (int i=0; i<24-num_stars; i++){
        bar.setBar(i, LED_OFF);
      }
      bar.writeDisplay();
      Serial.println();  // Newline after stars

    } else {
      Serial.println("Invalid RSSI – check connection");
    }
  }

  // Check if still connected
  if (!pClient->isConnected()) {
    connected = false;
    Serial.println("Acquiring Signal");
    bar.blinkRate(1);
    //lcd.clear();
    //lcd.print("Acquiring Signal");
    //lcd.setCursor(0, 1);
    //lcd.print("Searching...");
  }
}
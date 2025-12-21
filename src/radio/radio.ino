// ================================================
// PERIPHERAL DEVICE (Device 1: Advertises and accepts connections)
// Upload this to the first XIAO ESP32-C3
// It will print its BLE MAC address to Serial – copy it for the client code!
// ================================================

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Define a simple service UUID (can be anything unique)
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLEServer* pServer = NULL;
bool deviceConnected = false;

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

void setup() {
  pinMode(A0, INPUT); 
  Serial.begin(115200);
  delay(1500);  // For XIAO USB CDC

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
  // Nothing needed – just waits for connection
  if (deviceConnected) {
    // Optional: Do something while connected
    delay(1000);
  }
  uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction   
  }
  float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V
  Serial.println(Vbattf, 3);
  delay(1000);
}
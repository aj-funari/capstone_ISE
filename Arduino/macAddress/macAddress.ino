#include <Arduino.h>
#include <ArduinoBLE.h>

void setup() {
  Serial.begin(9600);

  if (BLE.begin(/*device name*/)) {
    Serial.println("Bluetooth initialized");
  } else {
    Serial.println("Error initializing Bluetooth");
  }

  // Serial.print("Arduino Nano connected on port: ");
  // Serial.println(Serial.port());

  // Serial.print("Peripheral MAC address: ");
  // Serial.println(BLE.address());
}

void loop() {
  Serial.print("Peripheral MAC address: ");
  Serial.println(BLE.address());
}

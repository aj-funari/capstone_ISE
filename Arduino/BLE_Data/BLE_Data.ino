/*
  BLE_Data

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED. The callback features of the
  library are used.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read, write, and notify
// BLEByteCharacteristic gyroCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharacteristic gyroCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);

float x, y, z;

void setup() {
  Serial.begin(9600);
  // while (!Serial);
  
  // pinMode(ledPin, OUTPUT); // use the LED pin as an output

  // begin IMU initialization
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
  }

  // begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  Serial.print("My BLE MAC:\t\t ");
  Serial.println(BLE.address());
  Serial.print("Service UUID:\t\t ");
  Serial.println(dataService.uuid());
  Serial.print("Characteristic UUID:\t ");
  Serial.println(gyroCharacteristic.uuid());
  Serial.println();

  // set the local name peripheral advertises
  BLE.setLocalName("ArduinoData");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(dataService);

  // add the characteristic to the service
  dataService.addCharacteristic(gyroCharacteristic);

  // add service
  BLE.addService(dataService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // event handler for when data is written to gyroCharacteristic
  // gyroCharacteristic.setEventHandler(BLEWritten, gyroCharacteristicWritten);
  // gyroCharacteristic.setEventHandler(BLERead, gyroCharacteristicRead);
  // gyroCharacteristic.setEventHandler(BLENotify, gyroCharacteristicNotify);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }

  char buffer[50];
  sprintf(buffer, "x: %.2f y: %.2f, z: %.2f", x, y, z);  // print out multiple variables into string
  gyroCharacteristic.writeValue(buffer);
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
} 

// void gyroCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
//   byte charValue = gyroCharacteristic.value();
//   Serial.print("Value written to characteristic: ");
//   Serial.println(charValue);

//   // if (IMU.gyroscopeAvailable()) {
//   //   IMU.readGyroscope(x, y, z);
//   //   Serial.println(x);
//   //   Serial.println(y);
//   //   Serial.println(z);
//   // }
// }

// void gyroCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
//   byte charValue = gyroCharacteristic.value();
//   Serial.print("Value read from characteristic: ");
//   Serial.println(charValue);
// }

// void gyroCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
//   Serial.println(gyroCharacteristic.value());
// }

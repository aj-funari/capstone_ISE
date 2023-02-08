/*
  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to read and write IMU data.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>  // Bluetooth library
#include <Arduino_LSM9DS1.h>  // IMU library 

#define BLE_UUID_LED                        "180A"

BLEService dataService( BLE_UUID_LED ); // BLE LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central 
BLECharacteristic gyro_Characteristic("2A58", BLERead | BLENotify, 50);  // Digital Output
// BLEFloatCharacteristic gyro_Characteristic("2A58", BLENotify);

float x, y, z;

void setup() {
  Serial.begin(9600);  // returns nothing 

  // Check if IMU is working

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);    
  }
  else
  {
    Serial.println("Initialized IMU!");
  }

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
    else
  {
    Serial.println("Initialized Bluetooth!");
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(dataService);

  /******************************************************
    Data is written and read from characteristics
      - add the characteristic to the service
  ******************************************************/
  dataService.addCharacteristic(gyro_Characteristic);  // 

  // add service
  BLE.addService(dataService);

  // set the initial value for the characeristic:
  gyro_Characteristic.writeValue("Test");  // Not sure if this works 

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {

  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);       // turn on the LED to indicate the connection

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:

    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
    }

    // Write x as float to characteristic
    // gyro_Characteristic.setValue(x);
    // gyro_Characteristic.writeValue(x);

    // Write x, y, z data as string to characteristic
    char buffer[50]; 
    sprintf(buffer, "x: %.2f y: %.2f, z: %.2f", x, y, z);  // print out multiple variables into string
    gyro_Characteristic.writeValue(buffer);
    
    }
    
    // when the central disconnects, print it out:
    Serial.println(F("Disconnected from central: "));
  }
}

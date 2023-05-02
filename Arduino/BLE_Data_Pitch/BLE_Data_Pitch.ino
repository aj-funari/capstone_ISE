/*
  BLE_Data_Pitch

  This script writes roll or pitch angle associated with the on board IMU and analog
  data to a Bluetooth Low Engery peripheral.  

*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// Bluetooth service
BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create gyro characteristic and allow remote device to read and notify
BLECharacteristic dataCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);  // data passed as strinng

// varaibles for roll and pitch calculation
float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      pitch, roll;

// variables for force transducers
int forceOne, forceTwo;
float voltageOne, voltageTwo;

void setup() {
  Serial.begin(9600);

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

  // Serial.print("My BLE MAC:\t\t ");
  // Serial.println(BLE.address());
  // Serial.print("Service UUID:\t\t ");
  // Serial.println(dataService.uuid());
  // Serial.print("Characteristic UUID:\t ");
  // Serial.println(dataCharacteristic.uuid());
  // Serial.println();

  // set the local name peripheral advertises
  BLE.setLocalName("ArduinoData");

  // set the UUID for the service
  BLE.setAdvertisedService(dataService);

  // add characteristic to Bluetooth service;
  dataService.addCharacteristic(dataCharacteristic);
  
  // add service
  BLE.addService(dataService);
  
  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();

  // calculate roll
  if (readIMU()) {
    roll = atan2(-accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180 / M_PI;
    pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;
    Serial.print("roll: ");
    Serial.print(roll);
    Serial.print(" ");
    Serial.print("pitch: ");
    Serial.println(pitch);
  }

  // read force from analog pins
  // forceOne = analogRead(A0);
  // voltageOne = forceOne / 5;
  // forceTwo = analogRead(A1);
  // voltageTwo = forceTwo / 5;

  // send 0 or 1 
  // if(voltageOne > 0)
  //   voltageOne = 1;
  // if(voltageTwo > 0)
  //   voltageTwo = 1;

  // Pass data to characteristic using array
  char buffer[10];  // maximim string length: "-xxx xxxx xxxx\0"
  sprintf(buffer, "%.0f %.0f %.0f", roll, pitch);  // print out multiple variables into strin
  
  // write data to characteristic
  dataCharacteristic.writeValue(buffer);
}

// central connected event handler
void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

// central disconnected event handler
void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
} 

// read accerometer and gyroscope
bool readIMU() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() ) {
    IMU.readAcceleration(accelX, accelY, accelZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    return true;  // return true if IMU is available
  }
  return false;  // return false if IMU is unavailable
}
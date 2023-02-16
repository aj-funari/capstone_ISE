/*
  BLE_Data_Raw

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to write raw force and IMU data. 

*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create gyro characteristic and allow remote device to read, write, and notify
BLECharacteristic dataCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);  // passing data as strinng

// variables for IMU (Gyroscope/Accelerometer)
float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ;             // units dps (degrees per second)

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

  Serial.print("My BLE MAC:\t\t ");
  Serial.println(BLE.address());
  Serial.print("Service UUID:\t\t ");
  Serial.println(dataService.uuid());
  Serial.print("Characteristic UUID:\t ");
  Serial.println(dataCharacteristic.uuid());
  Serial.println();

  // set the local name peripheral advertises
  BLE.setLocalName("ArduinoData");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(dataService);

  /* add characteristic to the service; */

  dataService.addCharacteristic(dataCharacteristic);

  // add service
  BLE.addService(dataService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // event handler for when data is written to gyroCharacteristic
  // dataCharacteristic.setEventHandler(BLEWritten, gyroCharacteristicWritten);
  // dataCharacteristic.setEventHandler(BLERead, gyroCharacteristicRead);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();

  readIMU();

  // read force from analog pins
  forceOne = analogRead(A0);
  voltageOne = forceOne * (3.3/1023);
  forceTwo = analogRead(A1);
  voltageTwo = forceTwo * (3.3/1023);
  // Serial.print("Digital force value reading = ");
  // Serial.println(val_force);
  // Serial.print("Digital voltage value reading = ");
  // Serial.println(voltage);

  // Pass data to characteristic using array
  char buffer[50];  // maximim string length: "-xxx xxxx xxxx\0" 
  sprintf(buffer, "%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", 
                                      voltageOne, voltageTwo,
                                      accelX, accelY, accelZ,
                                      gyroX, gyroY, gyroZ);  // print out multiple variables into string

  dataCharacteristic.writeValue(buffer);
}

/******************************************************
Bluetooth Functions
- One function to handle connection
- One function to handle disconnection
******************************************************/
/*
Will printing to the screen inside bluetooth handler slow
the code down?
"""
*/
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

/*******************************
Initial Measurement Unit (IMU) 
- Function to read IMU values 
*******************************/

bool readIMU() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() ) {
    IMU.readAcceleration(accelX, accelY, accelZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    return true;  // return true if IMU is available
  }
  return false;  // return false if IMU is unavailable
}

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
// #include <Math.h>

BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create gyro characteristic and allow remote device to read, write, and notify
// BLEByteCharacteristic gyroCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharacteristic gyroCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);

// variables to print gyroscope data
// float x, y, z;

// varaibles for pitch calculation
float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      gyroDriftX,        gyroDriftY,         gyroDriftZ,        // units dps
      gyroRoll,          gyroPitch,          gyroYaw,           // units degrees (expect major drift)
      gyroCorrectedRoll, gyroCorrectedPitch, gyroCorrectedYaw,  // units degrees (expect minor drift)
      accRoll,           accPitch,           accYaw,            // units degrees (roll and pitch noisy, yaw not possible)
      complementaryRoll, complementaryPitch, complementaryYaw;  // units degrees (excellent roll, pitch, yaw minor drift)

long lastTime;
long lastInterval;

// variables for force transducers
int val_force;
float voltage;

void setup() {
  Serial.begin(9600);

  // calibrate IMU  --> causing sketch to fail when uploading 
  // calibrateIMU(250, 250);
  // lastTime = micros();

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

  // if (IMU.gyroscopeAvailable()) {
  //   IMU.readGyroscope(x, y, z);
  //   // Serial.print(x);
  //   // Serial.print(" ");
  //   // Serial.print(y);
  //   // Serial.print(" ");
  //   // Serial.println(z);
  // }

  // obtain pitch using gyro and acclerometer
  if (readIMU()) {
    long currentTime = micros();
    lastInterval = currentTime - lastTime; // expecting this to be ~104Hz +- 4%
    lastTime = currentTime;

    doCalculations();
    // Serial.print(complementaryRoll);
    // Serial.print(',');
    // Serial.print(complementaryPitch);
    // Serial.print(',');
    // Serial.print(complementaryYaw);
    // Serial.println("");
  }

  // read force from analog pins
  val_force = analogRead(A0);
  voltage = val_force *(3.3/1023);
  Serial.print("Digital force value reading = ");
  Serial.println(val_force);
  Serial.print("Digital voltage value reading = ");
  Serial.println(voltage);

  // write data to bluetooth characteristic
  char buffer[50];
  // sprintf(buffer, "x: %.2f y: %.2f z: %.2f", x, y, z);
  sprintf(buffer, "X: %.2f Y: %.2f Z: %.2f", complementaryRoll, complementaryPitch, complementaryYaw);  // print out multiple variables into string
  gyroCharacteristic.writeValue(buffer);
}

/******************************************************
Bluetooth Functions
- One function to handle connection
- One function to handle disconnection
******************************************************/
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

/******************************************************
Initial Measurement Unit (IMU) Calculations
- One function to calibrate IMU
- One function to read IMU values
- One function to calculate pitch
- One function to print calculations 
******************************************************/
void calibrateIMU(int delayMillis, int calibrationMillis) {

  int calibrationCount = 0;

  delay(delayMillis); // to avoid shakes after pressing reset button

  float sumX, sumY, sumZ;
  int startTime = millis();
  while (millis() < startTime + calibrationMillis) {
    if (readIMU()) {
      // in an ideal world gyroX/Y/Z == 0, anything higher or lower represents drift
      sumX += gyroX;
      sumY += gyroY;
      sumZ += gyroZ;

      calibrationCount++;
    }
  }

  if (calibrationCount == 0) {
    Serial.println("Failed to calibrate");
  }

  gyroDriftX = sumX / calibrationCount;
  gyroDriftY = sumY / calibrationCount;
  gyroDriftZ = sumZ / calibrationCount;

}

bool readIMU() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() ) {
    IMU.readAcceleration(accelX, accelY, accelZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    return true;  // return true if IMU is available
  }
  return false;  // return false if IMU is unavailable
}

void doCalculations() {
  accRoll = atan2(accelY, accelZ) * 180 / M_PI;
  accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;

  float lastFrequency = (float) 1000000.0 / lastInterval;
  gyroRoll = gyroRoll + (gyroX / lastFrequency);
  gyroPitch = gyroPitch + (gyroY / lastFrequency);
  gyroYaw = gyroYaw + (gyroZ / lastFrequency);

  gyroCorrectedRoll = gyroCorrectedRoll + ((gyroX - gyroDriftX) / lastFrequency);
  gyroCorrectedPitch = gyroCorrectedPitch + ((gyroY - gyroDriftY) / lastFrequency);
  gyroCorrectedYaw = gyroCorrectedYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = complementaryRoll + ((gyroX - gyroDriftX) / lastFrequency);
  complementaryPitch = complementaryPitch + ((gyroY - gyroDriftY) / lastFrequency);
  complementaryYaw = complementaryYaw + ((gyroZ - gyroDriftZ) / lastFrequency);

  complementaryRoll = 0.98 * complementaryRoll + 0.02 * accRoll;
  complementaryPitch = 0.98 * complementaryPitch + 0.02 * accPitch;
}

// void printCalculations() {
//   //Serial.print(gyroRoll);
//   //Serial.print(',');
//   //Serial.print(gyroPitch);
//   //Serial.print(',');
//   //Serial.print(gyroYaw);
//   //Serial.print(',');
//   //Serial.print(gyroCorrectedRoll);
//   //Serial.print(',');
//   //Serial.print(gyroCorrectedPitch);
//   //Serial.print(',');
//   //Serial.print(gyroCorrectedYaw);
//   //Serial.print(',');
//   //Serial.print(accRoll);
//   //Serial.print(',');
//   //Serial.print(accPitch);
//   //Serial.print(',');
//   //Serial.print(accYaw);
//   //Serial.print(',');
//   Serial.print(complementaryRoll);
//   Serial.print(',');
//   Serial.print(complementaryPitch);
//   Serial.print(',');
//   Serial.print(complementaryYaw);
//   Serial.println("");
// }

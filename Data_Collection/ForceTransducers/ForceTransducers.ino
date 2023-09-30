#include <Wire.h>

#include <MadgwickAHRS.h>
#include <Arduino_LSM9DS1.h>
#include <Math.h>

/*float pitchFilteredOld;
Madgwick filter;
const float sensorRate = 104.00;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if(!IMU.begin())  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  filter.begin(sensorRate);
  Serial.println("Setup complete!");
}  

void loop() {
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  float pitch;
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro); 
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
    //Serial.println("depend");
    pitch = filter.getPitch(); 
    Serial.println(pitch);
    //float pitchFiltered = 0.1 * pitch + 0.9 * pitchFilteredOld; // low pass filter
    //Serial.println("pitch: " + String(pitchFiltered));
    //pitchFilteredOld = pitchFiltered;
  }
}*/


int val_pot;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  val_pot = analogRead(A1);
  Serial.print("The digitial value is: ");
  if (val_pot > 200){
    digitalWrite(LED,LOW);
  }
  else {
    digitalWrite(22,HIGH);
  }
  Serial.println(val_pot);
  delay(100);

}

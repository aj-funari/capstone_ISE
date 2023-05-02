int force_0, force_1, voltage_0, voltage_1;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
}
 
void loop(void) {
  force_0 = analogRead(A0);
  force_1 = analogRead(A1);
  // Serial.println("Digital value reading = ");
  // Serial.println(val_fsr);
  voltage_0 = force_0 *(3.3/1023);
  voltage_1 = force_1 *(3.3/1023);
  Serial.println(voltage_0);
  Serial.println(voltage_1);
  delay(100);
}

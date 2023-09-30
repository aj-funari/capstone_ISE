int val_fsr;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
}
 
void loop(void) {
  val_fsr = analogRead(A0);
  Serial.println("Digital value reading = ");
  Serial.println(val_fsr);
  float voltage = val_fsr *(3.3/1023);
  Serial.println(voltage);
  delay(100);
}

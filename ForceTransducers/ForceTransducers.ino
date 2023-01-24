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
    digitalWrite(22,LOW);
  }
  else {
    digitalWrite(22,HIGH);
  }
  Serial.println(val_pot);
  delay(100);

}

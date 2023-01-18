void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Initialize serial communication at 9600 baud

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!"); // Print "Hello, World!" over serial
  delay(1000); // Delay for 1 second  
}

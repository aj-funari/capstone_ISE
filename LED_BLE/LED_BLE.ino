/*
  LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>  // Bluetooth library

#define BLE_UUID_LED                        "180A"

BLEService ledService( BLE_UUID_LED ); // BLE LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("2A57", BLEWrite);  // Digital Output 

const int ledPin = LED_BUILTIN; // pin to use for the LED
float x, y, z;

void setup() {
  Serial.begin(9600);
  // Serial.print(Serial);
  while (!Serial);

  // set LED pin to output mode
  // pinMode(ledPin, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);  // when the central disconnects, turn off the LED
  digitalWrite(LEDR, HIGH);        // will turn the LED off
  digitalWrite(LEDG, HIGH);        // will turn the LED off
  digitalWrite(LEDB, HIGH);        // will turn the LED off

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);  // byte characteristic

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

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

    if (switchCharacteristic.written() ) {
    switch (switchCharacteristic.value()) {   // any value other than 0
      case 01:
        Serial.println("Red LED on");
        digitalWrite(LEDR, LOW);          // will turn the LED on
        digitalWrite(LEDG, HIGH);         // will turn the LED off
        digitalWrite(LEDB, HIGH);         // will turn the LED off
        break;
      case 02:
        Serial.println("Green LED on");
        digitalWrite(LEDR, HIGH);         // will turn the LED off
        digitalWrite(LEDG, LOW);          // will turn the LED on
        digitalWrite(LEDB, HIGH);         // will turn the LED off
        break;
      case 03:
        Serial.println("Blue LED on");
        digitalWrite(LEDR, HIGH);         // will turn the LED off
        digitalWrite(LEDG, HIGH);         // will turn the LED off
        digitalWrite(LEDB, LOW);          // will turn the LED on
        break;
      default:
        Serial.println(F("LEDs off"));
        digitalWrite(LEDR, HIGH);         // will turn the LED off
        digitalWrite(LEDG, HIGH);         // will turn the LED off
        digitalWrite(LEDB, HIGH);         // will turn the LED off
        break;
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH); 
  }
}

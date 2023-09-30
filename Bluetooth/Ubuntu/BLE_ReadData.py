"""
Python/Raspberry Pi BluetoothLE exercise:
using bluepy library

Work with Arduino Nano 33 BLE:
ArduinoBLE > Peripheral > Central_ReadData

Connect to Peripheral with matched MAC.
Check for service uuid and characteristic uuid,
if matched found: read data from characteristic
"""

from bluepy import btle
import time

# Must match with Peripheral
MAC = "a8:8f:15:63:27:d2"
SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214"
CHARACTERISTIC_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"

# Connect to Peripheral
print("Connect to:" + MAC)
dev = btle.Peripheral(MAC)
print("\n--- dev ----------------------------")
print(type(dev))
print(dev)

print("\n--- dev.services -------------------")
for svc in dev.services:
    print(str(svc))
    
print("\n------------------------------------")
print("Get Serice By UUID: " + SERVICE_UUID)
service_uuid = btle.UUID(SERVICE_UUID)
service = dev.getServiceByUUID(service_uuid)

print(service)
print("\n--- service.getCharacteristics() ---")
print(type(service.getCharacteristics()))
print(service.getCharacteristics())

#----------------------------------------------
characteristics = dev.getCharacteristics()
print("\n--- dev.getCharacteristics() -------")
print(type(characteristics))
print(characteristics)

while(dev):
    for char in characteristics:
        # print("----------")
        # print(type(char))
        # print(char)
        # print(char.uuid)
        if(char.uuid == CHARACTERISTIC_UUID ):
            # print("=== !CHARACTERISTIC_UUID matched! ==")
            # nanoRP2040_Char = char
            # print(char)
            # print(dir(char))
            # print(char.getDescriptors)
            # print(char.propNames)
            # print(char.properties)
            # print(type(char.read()))
            print(char.read())

# Disconnect with Peripheral 
# dev.disconnect()
# print("\n--- bye ---\n")     
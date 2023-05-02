"""
Python/Raspberry Pi BluetoothLE exercise:
using bluepy library

Work with Arduino Nano 33 BLE:
ArduinoBLE > Peripheral > Central_ReadData

Connect to Peripheral with matched MAC.
Check for service uuid and characteristic uuid,
if matched found: read data from characteristic
"""
import pyKey
from bluepy import btle


# Must match with Peripheral
#AJ Arduino MAC a8:8f:15:63:27:d2
#Actual Arduino MAC 66:46:6d:83:a5:c8
MAC = "66:46:6d:83:a5:c8"
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
        if(char.uuid == CHARACTERISTIC_UUID ):
            # (roll) (jump) (reset) 
            data = (char.read()).decode("utf-8")

            # Offset by 2 to bypass the b'
            i = 0
            #Run through a while loop to get all the pitch data, This is necessary since it is variable in size
            pitch = ""
            while(data[i] != " "):
                pitch += data[i]
                i += 1
            # We now have all of pitch, this is variable however, ranging from a size of 1 to 4
            # Now we need to grab the jump and reset button, these can only be size 1
            # Offset by 1 as if we don't we'll grab the whitespace
            jump = data[i+1]
            # NOTE an offset of 2 from the jump, this is to account for whitespace
            reset = data[i+3]
            
            # These values are turned into ints
            pitch_i = int(pitch)
            jump_i = int(jump)
            reset_i = int(reset)
            
            #Control loop
            if(pitch_i > 20):
                #Use pressKey ONLY for moving laterally
                # You MUST releaseKey if you use pressKey
                pyKey.pressKey("RIGHT")
            elif(pitch_i < -20):
                pyKey.pressKey("LEFT")
            else:
                pyKey.releaseKey("RIGHT")
                pyKey.releaseKey("LEFT")
            if(jump_i > 0):
                #press does a quick "tap" of the key, use for buttons
                pyKey.press("SPACEBAR")
            if(reset_i > 0):
                pyKey.press("r")

            

# Disconnect with Peripheral 
# dev.disconnect()
# print("\n--- bye ---\n")     

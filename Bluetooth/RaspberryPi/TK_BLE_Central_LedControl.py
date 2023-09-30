"""
Python/Raspberry Pi BluetoothLE exercise:
using bluepy library

Work with Arduino Nano RP2040 Connect example:
ArduinoBLE > Peripheral > CallbackLED

Connect to Peripheral with matched MAC.
Check for service uuid and characteristic uuid,
if matched found:
    Start GUI to control Nano RP2040 Connect onboard LED
"""

from bluepy import btle
import time
import tkinter as tk

#Have to match with Peripheral
MAC = "a8:8f:15:63:27:d2"
SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214"
CHARACTERISTIC_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"

nanoRP2040_Char = None

def toggle():
    if toggle_btn.config('relief')[-1] == 'sunken':
        toggle_btn.config(relief="raised")
        nanoRP2040_Char.write(bytes_OFF, True)
        toggle_btn['text'] = 'Turn LED ON'
    else:
        toggle_btn.config(relief="sunken")
        nanoRP2040_Char.write(bytes_ON, True)
        toggle_btn['text'] = 'Turn LED OFF'

print("Hello")

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
    
for char in characteristics:
    print("----------")
    print(type(char))
    print(char)
    print(char.uuid)
    if(char.uuid == CHARACTERISTIC_UUID ):
        print("=== !CHARACTERISTIC_UUID matched! ==")
        nanoRP2040_Char = char
        print(char)
        print(dir(char))
        #print(char.getDescriptors)
        #print(char.propNames)
        #print(char.properties)
        #print(type(char.read()))
        print(char.read())
        
bytes_ON = b'\x01'
bytes_OFF = b'\x00'

if nanoRP2040_Char != None:
    print("\nnanoRP2040_Char found")
    print(nanoRP2040_Char)
    
    root = tk.Tk()
    label = tk.Label( root, text="Toggle button to Turn ON/OFF the nano RP040 Connect LED")
    label.pack(pady=10)
    toggle_btn = tk.Button(text="Turn LED ON", width=12, relief="raised", command=toggle)

    toggle_btn.pack(pady=10)
    root.geometry("500x200")

    #Place tkinter window center
    root.eval('tk::PlaceWindow %s center' % root.winfo_pathname(root.winfo_id()))
    root.title("arduino-er.blogspot.com")
    root.mainloop()
else:
    print("\nnanoRP2040_Char NOT found!")
#=============================================
dev.disconnect()
print("\n--- bye ---\n")
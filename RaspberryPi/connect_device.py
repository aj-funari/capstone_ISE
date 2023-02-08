import bluepy.btle as btle
from bluepy.btle import Scanner, Peripheral, DefaultDelegate

# Device A8:8F:15:63:27:D2 Name: Nano 33 BLE (public)

device = Peripheral("A8:8F:15:63:27:D2", addrType=btle.ADDR_TYPE_PUBLIC)
# device = Peripheral("A8:8F:15:63:27:D2", btle.ADDR_TYPE_RANDOM)

device.connect("A8:8F:15:63:27:D2")

print("Device Connected!")

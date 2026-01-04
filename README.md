![](img/logo.png)

# Radio Ranger

Radio Ranger is a proximity sensing device. The LED meter increases in proportion to how close the two devices are to one another.

After the devices are turned on they will begin searching (single red light sweeping).
When one device discovers the other, it will display how close it is to the other device:

Far ← | Red | Yellow | Green | → Close

## Games

    • Blood Hound
        ◦ Hide one device and search with it with the other
    • Radio Tag
        ◦ Tag if green
    • Marco Polo
    • Use your imagination


## Battery Charge

Flashing indicates battery level 
    • No Flash = Charged
    • Slow Flash = Battery Low
    • Fast Flash = Charge Immediately

To charge plug in a usb-c cable to a standard 5v receptacle. 
The device must be turned ON in order to charge.

## Parts

- [XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [Adafruit LEDBAR](https://www.adafruit.com/product/1721)
- switch/battery

## XIAO ESP32C3 PINOUT

```
BATT(+/-)-|A0  USB  5V|-
         -|D1      GND|-LED(G)
         -|D2      3V3|-LED(VCC)
         -|D3      D10|-
LED(SDA) -|D4       D9|-
LED(SCL) -|D5       D8|-
         -|D6       D7|-
```

Pin A0 is connected to the positive and negative battery pads via 220K resistors for the battery voltage reading.
The battery is connected to the ESP32 battery pads via the switch.

![](img/IMG20251222212620.jpg)

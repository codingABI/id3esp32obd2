# id3esp32obd2

## License and copyright
This project is licensed under the terms of the 2-Clause BSD License [Copyright (c) 2023 codingABI](LICENSE). 

## Appendix
### OBD2 and power up
The VW ID3 has an ODB2 connector below the steering wheel.
![ID3 OBD2 Connector](assets/images/id3obd2ConnectorFemale.jpg)
The id3esp32obd2 uses pin 16 (=12V) and pin 5 (=GND) to power up the device and pin 6 (=CAN High) and 14 (=CAN Low) to access the CAN bus. 
![OBD2 connector male](assets/images/obd2ConnectorMale.jpg)
The 12V line can be switch on and off with a toggle switch.

![Schematic](assets/images/Schematic.png)

[Arduino-Sketch](/id3esp32obd2/id3esp32obd2.ino)

![Device without case](assets/images/Device.jpg)
![Device with case](assets/images/DeviceWithCase.jpg)
![Device with open case](assets/images/DeviceWithOpenCase.jpg)
![Perfboard](assets/images/Perfboard.jpg)
### Android App
To show the ID3 data id3esp32obd2 need an Android phone with the App ![id3esp32obd2 app](id3esp32obd2.apk). This app is a small app made with https://appinventor.mit.edu/ (Source code  ![App source code](id3esp32obd2.aia))

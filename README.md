# rvFridgeTempSensors

Due to inconsistencies with cooling in the refrigeration system, developing a device to monitor temperatures in an RV refrigerator and freezer combination unit.  Will display the current temperature of each unit, along with the minimum, maximum, average, and the ability to alert when a threshold has been breached.

Currently utilizing the following devices:
- 1x Adafruit Huzzah ESP8266
- 2x Adafruit MCP9808 Temperature Sensors (w/ a jumper from VIN to A0 on one of them to allow it to be addressed as 0x19)
- 1x Adafruit SH110X 128x64 OLED Screen

End state will include a 3D printed enclosure to mount the device on the outside of the refrigerator for easy visual monitoring.

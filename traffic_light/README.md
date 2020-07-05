# traffic_light
Once upon a time I bought a traffic light for about 1.5 years of Amazon Prime.

This is all designed for an ESP32, though it should only take minimal modification to install on an ESP8266

## Useful Commands
These are pretty specific to my setup, but with a bit of research this could work for you.

### Compile and Upload
`arduino-cli compile --warnings all -t --fqbn esp32:esp32:node32s:baud=921600 traffic_light/ -p /dev/ttyUSB0 -t -u`

### Monitor Serial
`screen /dev/ttyUSB0 115200`

## Notes
* Install `pyserial` in a Python venv and run `arduino-cli` and `arduino` from it.
* Some random async webserver stuff has to be manually installed (it's not installable through the Arduino IDE package manager)

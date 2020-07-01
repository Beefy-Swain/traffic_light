# traffic_light
Once upon a time I bought a traffic light for about 1.5 years of Amazon Prime.

## Useful Commands
These are pretty specific to my setup, but with a bit of research you could probably make this work for you.

### Compile
`arduino-cli compile -b esp32:esp32:node32s traffic_light/`

### Flash to ESP
`arduino-cli upload traffic_light/ -b esp32:esp32:node32s -p /dev/ttyUSB0`

### Monitor Serial
`screen /dev/ttyUSB0 115200`

## Notes
* Install `pyserial` in a Python venv and run `arduino-cli` and `arduino` from it.
* Some random async webserver stuff has to be manually installed (it's not installable through the Arduino IDE package manager)
* The node32s requires the boot button to be pressed manually each time you re-flash the sketch

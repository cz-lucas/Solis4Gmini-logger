



### Solis 4G mini logger
### Features
- MQTT
- Webinterface
- Write data in an influxdb-database
- Send data to [PVOutput](https://pvoutput.org/)

### Used Hardware
- Wemos d1 mini
- Max485
- 2 leds (blue, yellow)
- 2 100 Ohm-resistors

### Schematic
![Schematic](https://github.com/10k-resistor/Solis4Gmini-logger/blob/master/docs/files/img/schematic.jpg "Picture from schematic")

### Steps to build - software
0. Install VSCode and platformIO
1. Download or clone this repository
2. Rename the file src/config.h.example to src/config.h
3. Configure it in src/config.h
4. Compile, upload and test your device.
#### More detailed instructions: [https://10k-resistor.github.io/Solis4Gmini-logger/instructions/detailed/](https://10k-resistor.github.io/Solis4Gmini-logger/instructions/detailed/)

 
### Status leds
| Status                 | Yellow                          | Blue                          |
|------------------------|---------------------------------|-------------------------------|
| Inverter off           | off                             | Led flashes (0,5s on 20s off) |
| Inverter on            | off                             | on                            |
| sends data via mqtt    | on                              | on                            |
| Sends data to pvoutput | Led flashes (0,5s on,0,5s off)  | on                            |
| Connects with wifi     | Led flashes (0,5s on, 0,5s off) | off                           |
| Wifi connection failed | On                              | off                           |

### Known problems
 - ESP sometimes crashes while ntp sync

ESP-32 / Tessel IR Remote
====================

This is a work in-progress IR remote that currently serves as a case study for development of the [Reach open source project](https://github.com/tessel/reach-wg).
To get up and running with this project, you'll need an ESP32 Thing dev board from Sparkfun and a Tessel IR module.

Much of this code is based on [examples](https://github.com/espressif/esp-idf/tree/master/examples) provided by Espressif.

## Hookup Guide
| IR Module | ESP32 Thing |
| --------- | ----------- |
| GND | GND |
| 3.3v | 3.3v |
| SCL | N/A |
| SDA | N/A |
| SCK | IO 19 |
| MISO | IO 25 |
| MOSI | IO 23 |
| G1 | IO 22 |
| G2 | IO 18 |
| G3 | IO 5 |

## GATT attributes
There is one GATT service with the UUID `CFFBCCBA-E23B-4E6C-B5D8-73D97BFF0000`, and that service has one characteristic with the UUID `CFFBCCBA-E23B-4E6C-B5D8-73D97BFF1000`.

For help getting the ESP32 Thing set up, see the getting started guide [here](https://github.com/tessel/reach-wg/tree/master/firmware-esp32).

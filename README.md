<h1 align="center">ESP32 Weather Detection</h1>

<p align="center">
  A live weather station built on the ESP32. It reads temperature, humidity and air pressure,
  then shows them on an OLED display and the Serial Plotter.
</p>

<p align="center">
  <img alt="ESP32" src="https://img.shields.io/badge/board-ESP32-blue">
  <img alt="Arduino" src="https://img.shields.io/badge/framework-Arduino-00979D">
  <img alt="Sensors" src="https://img.shields.io/badge/sensors-DHT22%20%7C%20BMP280-orange">
  <img alt="Display" src="https://img.shields.io/badge/display-SSD1306%20OLED-lightgrey">
  <img alt="License" src="https://img.shields.io/badge/license-MIT-green">
</p>

<p align="center">
  <img src="media/demo.gif" alt="Demo of the ESP32 weather station circuit" width="640">
</p>

> **Note:** This project was built and tested in the [BenchTwin](https://benchtwin.com) simulator.
> The same wiring and code are meant for a real ESP32 DevKit V1.

---

## Features

- Temperature and humidity from a **DHT22**
- Air pressure from a **BMP280**
- Live readings on a **0.96" SSD1306 OLED** (128x64, I2C)
- BMP280 and OLED share one I2C bus using different addresses
- Updates automatically every 2 seconds
- Skips failed sensor reads instead of showing wrong values
- Prints `name:value` pairs that graph directly in the Serial Plotter

## Hardware

| Component | Purpose |
|---|---|
| ESP32 DevKit V1 (DOIT) | Microcontroller |
| DHT22 (AM2302) | Temperature and humidity |
| BMP280 | Air pressure |
| SSD1306 OLED, I2C | Display |
| Breadboard + jumper wires | Connections |

## Circuit

<p align="center">
  <img src="images/circuit.png" alt="Breadboard circuit" width="720">
</p>

### Wiring

| Device pin | ESP32 pin |
|---|---|
| DHT22 VCC | 3V3 |
| DHT22 GND | GND |
| DHT22 DATA | **GPIO 4** |
| BMP280 VCC | 3V3 |
| BMP280 GND | GND |
| BMP280 SDA | **GPIO 21** |
| BMP280 SCL | **GPIO 22** |
| OLED VCC | 3V3 |
| OLED GND | GND |
| OLED SDA | **GPIO 21** |
| OLED SCL | **GPIO 22** |

| I2C device | Address |
|---|---|
| BMP280 | `0x76` (some modules use `0x77`) |
| OLED | `0x3C` (some modules use `0x3D`) |

### Schematic

<p align="center">
  <img src="images/schematic.png" alt="Circuit schematic" width="800">
</p>

## Getting started

1. Wire the circuit as shown above.
2. Install these libraries in the Arduino IDE (Tools > Manage Libraries):
   - DHT sensor library (Adafruit)
   - Adafruit BMP280 Library
   - Adafruit SSD1306
   - Adafruit GFX Library
3. Open [`weather_station/weather_station.ino`](weather_station/weather_station.ino).
4. Select the board **DOIT ESP32 DEVKIT V1** and the correct port.
5. Click **Upload**. If it hangs on "Connecting...", hold the **BOOT** button.
6. Open the Serial Monitor at **115200 baud**, or the Serial Plotter, to see the values.

## Results

### OLED output

<p align="center">
  <img src="images/oled-output.png" alt="OLED showing temperature, humidity and pressure" width="800">
</p>

### Signal check

D4 carries the DHT22 data line. D21 and D22 carry the I2C bus (SDA and SCL). Activity on all three lines confirms the sensors are communicating with the ESP32.

<p align="center">
  <img src="images/pin-monitor.png" alt="Pin monitor showing D4, D21 and D22 activity" width="800">
</p>

Full demo video: [`media/demo.mp4`](media/demo.mp4)

## How it works

- `setup()` starts serial, I2C, the DHT22, the BMP280 and the OLED.
- `loop()` uses `millis()` to take a reading every 2 seconds without blocking.
- The DHT22 needs about 2 seconds between reads, so sampling faster causes failed readings.
- If a read fails, that cycle is skipped and the next one tries again.
- Pressure is read in Pascals and divided by 100 to show hPa.

## Troubleshooting

| Problem | Likely cause and fix |
|---|---|
| `DHT22: read failed` | Data wire not on GPIO 4, or loose power or ground |
| `BMP280 not found` | SDA/SCL swapped, or try address `0x77` |
| `OLED not found` | Check wiring, or try address `0x3D` |
| OLED stays blank | Make sure `display.display()` is called and the DHT22 is reading |
| Upload hangs on "Connecting..." | Hold the BOOT button on the ESP32 while uploading |

## Project structure

```
esp32-weather-detection/
├── weather_station/
│   └── weather_station.ino
├── images/
│   ├── circuit.png
│   ├── schematic.png
│   ├── oled-output.png
│   └── pin-monitor.png
├── media/
│   ├── demo.gif
│   └── demo.mp4
├── LICENSE
└── README.md
```

## Future improvements

- Show the readings on a web page over Wi-Fi
- Send data to a cloud dashboard (ThingSpeak or Blynk)
- Log data over time
- Calculate heat index and estimated altitude
- Build and test on real hardware

## License

Released under the [MIT License](LICENSE).

## Author

**Navneet Santhosh**

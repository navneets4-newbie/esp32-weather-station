/*
  ESP32 Weather Detection
  -----------------------
  Reads temperature + humidity (DHT22) and air pressure (BMP280),
  shows them on a 0.96" SSD1306 OLED and prints name:value pairs
  for the Serial Plotter.

  Wiring:
    DHT22  DATA -> GPIO 4      (VCC -> 3V3, GND -> GND)
    BMP280 SDA  -> GPIO 21     SCL -> GPIO 22   (VCC -> 3V3, GND -> GND)
    OLED   SDA  -> GPIO 21     SCL -> GPIO 22   (VCC -> 3V3, GND -> GND)

  I2C addresses: BMP280 = 0x76 (or 0x77), OLED = 0x3C (or 0x3D)
*/

#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>

const int DHT22_PIN = 4;
const unsigned long READ_INTERVAL_MS = 2000;  // DHT22 needs ~2 s between reads

DHT dht22(DHT22_PIN, DHT22);
Adafruit_BMP280 bmp280;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

bool bmpOK = false;
bool oledOK = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht22.begin();
  bmpOK = bmp280.begin(0x76);                          // try 0x77 if this fails
  oledOK = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // try 0x3D if this fails
  if (!bmpOK)  Serial.println("BMP280 not found. Check wiring and address.");
  if (!oledOK) Serial.println("OLED not found. Check wiring and address.");
}

void loop() {
  static unsigned long lastRead = 0;
  if (millis() - lastRead >= READ_INTERVAL_MS) {
    lastRead = millis();

    float t = dht22.readTemperature();
    float h = dht22.readHumidity();
    if (isnan(t) || isnan(h) || !bmpOK) return;   // skip bad reads
    float p = bmp280.readPressure() / 100.0;      // Pa -> hPa

    // Serial Plotter output
    Serial.print("temp:");      Serial.print(t);
    Serial.print(",humidity:"); Serial.print(h);
    Serial.print(",pressure:"); Serial.println(p);

    // OLED output
    if (oledOK) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Weather Station");
      display.setCursor(0, 20);
      display.print("Temp: ");     display.print(t, 1); display.println(" C");
      display.print("Humidity: "); display.print(h, 0); display.println(" %");
      display.print("Press: ");    display.print(p, 0); display.println(" hPa");
      display.display();
    }
  }
}

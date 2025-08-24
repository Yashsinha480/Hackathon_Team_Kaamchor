#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_VL53L0X lox;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println(F("VL53L0X long-range attempt"));

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1) {}
  }

  lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);

  #if defined(Adafruit_VL53L0X_H)
  bool ok = lox.setMeasurementTimingBudgetMicroSeconds(200000); 
  Serial.print("Timing budget set: "); Serial.println(ok ? "OK" : "NO");
  #endif

  Serial.println(F("Init done."));
}

void loop() {
  VL53L0X_RangingMeasurementData_t m;
  lox.rangingTest(&m, false);

  if (m.RangeStatus != 4) {
    Serial.print(F("Distance: "));
    Serial.print(m.RangeMilliMeter);
    Serial.println(F(" mm"));
  } else {
    Serial.println(F("Out of Range"));
  }

  delay(200); 
}

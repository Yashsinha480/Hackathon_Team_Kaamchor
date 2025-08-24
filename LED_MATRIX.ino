#include <MD_MAX72xx.h>
#include <SPI.h>

// Pin definitions for the LED matrix (MAX7219)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 3
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Pin definitions for the LDR sensors
#define LDR_LEFT_PIN 34
#define LDR_RIGHT_PIN 35

// Individual thresholds for each LDR based on your new calibration
#define LDR_LEFT_THRESHOLD 1500  // Example value, adjust as needed
#define LDR_RIGHT_THRESHOLD 1500 // Example value, adjust as needed

// General threshold for very bright ambient light
#define FLOODLIGHT_THRESHOLD 150

void setup() {
  Serial.begin(115200);
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5); // Set brightness
  turnOnAllMatrices();
}

void loop() {
  int leftLdrValue = analogRead(LDR_LEFT_PIN);
  int rightLdrValue = analogRead(LDR_RIGHT_PIN);

  // Debugging: Print values to serial monitor
  Serial.print("Left LDR: ");
  Serial.print(leftLdrValue);
  Serial.print(" | Right LDR: ");
  Serial.println(rightLdrValue);

  int averageLdrValue = (leftLdrValue + rightLdrValue) / 2;

  // Check for "floodlight" conditions (both sensors are very bright)
  if (averageLdrValue < FLOODLIGHT_THRESHOLD) {
    turnOffAllMatrices();
  } else {
    turnOnAllMatrices();

    // Now, use individual thresholds to detect the flashlight on each side.
    // Use your calibrated values here (e.g., if Left LDR threshold is 90, use < 90).
    if (leftLdrValue < LDR_LEFT_THRESHOLD) {
      turnOffMatrix(2);
    }
    if (rightLdrValue < LDR_RIGHT_THRESHOLD) {
      turnOffMatrix(0);
    }
  }
  delay(100);
}

// Helper functions (same as before)
void turnOffMatrix(int deviceIndex) {
  for (int row = 0; row < 8; row++) {
    mx.setRow(deviceIndex, row, 0x00);
  }
}

void turnOnMatrix(int deviceIndex) {
  for (int row = 0; row < 8; row++) {
    mx.setRow(deviceIndex, row, 0xFF);
  }
}

void turnOffAllMatrices() {
  for (int i = 0; i < MAX_DEVICES; i++) {
    turnOffMatrix(i);
  }
}

void turnOnAllMatrices() {
  for (int i = 0; i < MAX_DEVICES; i++) {
    turnOnMatrix(i);
  }
}

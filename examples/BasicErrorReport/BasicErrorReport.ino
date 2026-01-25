#include <BTErrRep.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("BTErrRep Basic Example");
  
  // Initialize BLE
  NimBLEDevice::init("BTErrRep Example");
  NimBLEDevice::createServer();
  
  // Start the Error Report Service
  BTErrRep.startService();
  
  // Start advertising
  NimBLEDevice::getAdvertising()->start();
  
  Serial.println("Error Report Service started");
  Serial.println("Advertising started");
  
  // Simulate various error conditions
  delay(2000);
  
  // Report a mild alert
  Serial.println("Reporting mild alert: Sensor calibration needed");
  BTErrRep.reportError(AlertLevel::MILD_ALERT, 100, "Sensor calibration needed");
  
  delay(3000);
  
  // Report a high alert
  Serial.println("Reporting high alert: Temperature critical");
  BTErrRep.reportError(AlertLevel::HIGH_ALERT, 200, "Temperature critical");
  
  delay(3000);
  
  // Clear the alert
  Serial.println("Clearing alert: System normal");
  BTErrRep.reportError(AlertLevel::NO_ALERT, 0, "System normal");
}

void loop() {
  // Simulate periodic system checks
  delay(10000);
  
  // Simulate random error
  if (random(100) < 10) { // 10% chance
    Serial.println("Random error: Low battery warning");
    BTErrRep.reportError(AlertLevel::MILD_ALERT, 300, "Low battery warning");
  }
} 
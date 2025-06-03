#include <BTErrRep.h>

BTErrRep errorReport(nullptr);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("BTErrRep Serial Command Example");
  Serial.println("=================================");
  
  // Initialize BLE
  NimBLEDevice::init("Error Reporter");
  
  // Create Error Report Service
  errorReport = BTErrRep(NimBLEDevice::createServer());
  
  // Start the service
  errorReport.startService();
  
  // Start advertising
  NimBLEDevice::getAdvertising()->start();
  
  Serial.println("Error Report Service started and advertising");
  Serial.println("");
  printHelp();
}

void loop() {
  // Check for serial commands
  if (Serial.available()) {
    String command = Serial.readString();
    command.trim();
    command.toLowerCase();
    
    if (command == "help") {
      printHelp();
    }
    else if (command == "clear") {
      Serial.println("Clearing all alerts...");
      errorReport.reportError(AlertLevel::NO_ALERT, 3000, "System normal");
      Serial.println("Alert cleared");
    }
    else if (command == "low") {
      Serial.println("Reporting low severity error...");
      errorReport.reportError(AlertLevel::MILD_ALERT, 3001, "Low priority warning");
      Serial.println("Low severity alert sent");
    }
    else if (command == "high") {
      Serial.println("Reporting high severity error...");
      errorReport.reportError(AlertLevel::HIGH_ALERT, 3002, "Critical system error");
      Serial.println("High severity alert sent");
    }
    else if (command == "battery") {
      Serial.println("Reporting battery warning...");
      errorReport.reportError(AlertLevel::MILD_ALERT, 3003, "Battery level low");
      Serial.println("Battery warning sent");
    }
    else if (command == "temp") {
      Serial.println("Reporting temperature alert...");
      errorReport.reportError(AlertLevel::HIGH_ALERT, 3004, "Temperature critical");
      Serial.println("Temperature alert sent");
    }
    else if (command == "memory") {
      Serial.println("Reporting memory warning...");
      errorReport.reportError(AlertLevel::MILD_ALERT, 3005, "Memory usage high");
      Serial.println("Memory warning sent");
    }
    else if (command == "network") {
      Serial.println("Reporting network error...");
      errorReport.reportError(AlertLevel::HIGH_ALERT, 3006, "Network connection lost");
      Serial.println("Network error sent");
    }
    else if (command == "sensor") {
      Serial.println("Reporting sensor failure...");
      errorReport.reportError(AlertLevel::HIGH_ALERT, 3007, "Sensor malfunction detected");
      Serial.println("Sensor failure sent");
    }
    else if (command == "custom") {
      Serial.println("Enter custom error message:");
      while (!Serial.available()) delay(10);
      String customMessage = Serial.readString();
      customMessage.trim();
      
      Serial.println("Reporting custom error...");
      errorReport.reportError(AlertLevel::MILD_ALERT, 3099, customMessage);
      Serial.println("Custom error sent: " + customMessage);
    }
    else if (command.length() > 0) {
      Serial.println("Unknown command: " + command);
      Serial.println("Type 'help' for available commands");
    }
  }
  
  delay(100);
}

void printHelp() {
  Serial.println("Available Commands:");
  Serial.println("==================");
  Serial.println("help     - Show this help message");
  Serial.println("clear    - Clear all alerts (NO_ALERT)");
  Serial.println("low      - Send low severity alert");
  Serial.println("high     - Send high severity alert");
  Serial.println("battery  - Send battery warning");
  Serial.println("temp     - Send temperature alert");
  Serial.println("memory   - Send memory warning");
  Serial.println("network  - Send network error");
  Serial.println("sensor   - Send sensor failure");
  Serial.println("custom   - Send custom error message");
  Serial.println("");
  Serial.println("Simply type a command and press Enter");
  Serial.println("");
} 
#include <NimBLEDevice.h>
#include "ble_crash_dump_service.h"

// Example BLE service and characteristic
const NimBLEUUID SERVICE_UUID        ("906374b1-bd94-4761-a4a6-af96723f5cbd");
const NimBLEUUID CHARACTERISTIC_UUID ("2ae00b3c-fca8-42e5-a3ce-f262816684cf");

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("BTErrRep Crash Dump Example");
  Serial.println("This demonstrates ESP32 core dump analysis and crash reporting");
  
  // Initialize BLE
  NimBLEDevice::init("Crash Dump Monitor");
  
  // Create the server
  NimBLEServer *server = NimBLEDevice::createServer();
  
  // Create the main service for your application
  NimBLEService *mainService = server->createService(SERVICE_UUID);
  NimBLECharacteristic *characteristic = mainService->createCharacteristic(
    CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
  );
  characteristic->setValue("Crash Monitor");
  mainService->start();
  
  // Start crash dump service - this will check for previous crashes and report them
  bleStartCrashDumpService();
  
  // Start advertising
  NimBLEDevice::getAdvertising()->start();
  
  Serial.println("Main service and Crash Dump Service started");
  Serial.println("Advertising started");
  Serial.println("To test crash detection, use Serial Monitor commands:");
  Serial.println("  'crash' - trigger a panic crash");
  Serial.println("  'assert' - trigger assertion failure");
  Serial.println("  'wdt' - trigger watchdog timeout");
}

void loop() {
  // Check for serial commands to trigger crashes for testing
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    
    if (command == "crash") {
      Serial.println("Triggering panic crash...");
      delay(100);
      abort(); // This will cause a panic
    }
    else if (command == "assert") {
      Serial.println("Triggering assertion failure...");
      delay(100);
      assert(false); // This will cause an assertion failure
    }
    else if (command == "wdt") {
      Serial.println("Triggering watchdog timeout...");
      delay(100);
      // Disable interrupts to trigger watchdog
      portDISABLE_INTERRUPTS();
      while (true) { /* infinite loop */ }
    }
    else {
      Serial.println("Unknown command. Available: 'crash', 'assert', 'wdt'");
    }
  }
  
  delay(1000);
} 
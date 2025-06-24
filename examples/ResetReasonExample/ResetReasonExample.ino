#include <NimBLEDevice.h>
#include "ble_error_report_service.h"

// Example BLE service and characteristic
const NimBLEUUID SERVICE_UUID        ("806374b1-bd94-4761-a4a6-af96723f5cbd");
const NimBLEUUID CHARACTERISTIC_UUID ("1ae00b3c-fca8-42e5-a3ce-f262816684cf");

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("BTErrRep Reset Reason Example");
  Serial.println("This demonstrates ESP32 reset reason detection and error reporting");
  
  // Initialize BLE
  NimBLEDevice::init("Reset Reason Monitor");
  
  // Create the server
  NimBLEServer *server = NimBLEDevice::createServer();
  
  // Create the main service for your application
  NimBLEService *mainService = server->createService(SERVICE_UUID);
  NimBLECharacteristic *characteristic = mainService->createCharacteristic(
    CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
  );
  characteristic->setValue("Reset Monitor");
  mainService->start();
  
  // Start error reporting service - this will check for reset reasons and report them
  bleStartErrorReportService();
  
  // Start advertising
  NimBLEDevice::getAdvertising()->start();
  
  Serial.println("Main service and Reset Reason Error Report Service started");
  Serial.println("Advertising started");
  Serial.println("Error report service will check ESP32 reset reasons on startup");
}

void loop() {
  // Simulate normal operation
  delay(5000);
  
  // Optional: You can report additional errors during runtime
  // BTErrRep errorReport(NimBLEDevice::getServer());
  // errorReport.reportError(AlertLevel::MILD_ALERT, 2000, "Runtime error example");
} 
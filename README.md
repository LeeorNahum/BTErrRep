# BTErrRep - Bluetooth Error Reporting Library

BTErrRep is a library for ESP32 that implements a Bluetooth Error Report Service. It provides a simple interface to report errors and alerts over BLE, including alert levels, emergency IDs, emergency text, and device timestamps. The library is based on the NimBLE-Arduino library.

## Features

- **Alert Level Notification**: Subscribe to alert level changes with automatic notification
- **Emergency Information**: Read emergency ID and descriptive text
- **Device Timestamps**: Automatic timestamping with current millis() or custom timestamps
- **Type-Safe API**: Uses enum for alert levels to prevent invalid values
- **BLE Standard Compliant**: Uses official BLE Alert Notification Service UUIDs

## Installation

### Arduino IDE

1. Download the library as a ZIP file.
2. Open the Arduino IDE.
3. Go to `Sketch` > `Include Library` > `Add .ZIP Library`.
4. Select the downloaded ZIP file.

### PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps =
  https://github.com/LeeorNahum/BTErrRep.git
```

## Usage

The [BasicErrorReport](examples/BasicErrorReport/BasicErrorReport.ino) example shows simple error reporting:

```cpp
#include <BTErrRep.h>

void setup() {
  // Initialize BLE
  NimBLEDevice::init("Error Reporter");
  
  // Create Error Report Service
  BTErrRep errorReport(NimBLEDevice::createServer());
  errorReport.startService();
  
  // Start advertising
  NimBLEDevice::getAdvertising()->start();
  
  // Report errors using type-safe enum
  errorReport.reportError(AlertLevel::MILD_ALERT, 100, "Sensor calibration needed");
  errorReport.reportError(AlertLevel::HIGH_ALERT, 200, "Temperature critical");
  errorReport.reportError(AlertLevel::NO_ALERT, 0, "System normal");
}
```

## Alert Levels

The library uses a type-safe enum for alert levels based on the BLE Alert Notification Service specification:

```cpp
enum class AlertLevel : uint8_t {
  NO_ALERT = 0x00,     // No alerting shall be done on this device
  MILD_ALERT = 0x01,   // The device shall alert (implementation specific)
  HIGH_ALERT = 0x02    // The device shall alert in the strongest possible way
};
```

## API Reference

### Constructor

```cpp
BTErrRep(NimBLEServer* server)
```

Creates a new BTErrRep instance using the provided BLE server.

### Methods

```cpp
void startService()
```

Starts the error report service. Must be called before using `reportError()`.

```cpp
void reportError(AlertLevel alert_level, 
                uint16_t emergency_id = 0, 
                const String& emergency_text = "", 
                uint32_t timestamp = millis())
```

Reports an error with the specified parameters:

- `alert_level`: Alert level using the `AlertLevel` enum
- `emergency_id`: Optional emergency identifier (default: 0)
- `emergency_text`: Optional error description (default: empty)
- `timestamp`: Optional timestamp (default: current millis())

## BLE Characteristics

The service exposes four characteristics:

| Characteristic | UUID | Properties | Description |
|----------------|------|------------|-------------|
| Alert Level | 2A06 | Read, Notify | Current alert level (triggers notifications) |
| Emergency ID | 2B2D | Read | Numeric emergency identifier |
| Emergency Text | 2B2E | Read | Human-readable error description |
| Device Time | 2B90 | Read | Timestamp when error occurred |

## Client Usage Pattern

1. **Subscribe** to the Alert Level characteristic for notifications
2. **Wait** for notification when an error occurs
3. **Read** Emergency ID, Emergency Text, and Device Time for details

## Requirements

- ESP32 microcontroller
- NimBLE-Arduino library
- Arduino framework or ESP-IDF

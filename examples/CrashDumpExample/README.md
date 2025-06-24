# BTErrRep Crash Dump Example

This example demonstrates how to use the BTErrRep library with ESP-IDF's core dump system to detect and report previous crashes via Bluetooth.

## Features

- Automatic detection of previous crashes on startup
- Detailed crash analysis with panic reason and task information
- BLE reporting of crash details
- Interactive crash triggering for testing
- Integration with ESP-IDF core dump system

## Setup Instructions

### 1. Configure Partition Table

This example requires a custom partition table to allocate space for core dumps. The `partitions_coredump.csv` file is included in this example.

### 2. PlatformIO Configuration

Add to your `platformio.ini`:

```ini
[env:your-board]
platform = espressif32
board = your-board-name
framework = espidf, arduino

# Essential: Enable core dump partition
board_build.partitions = partitions_coredump.csv

lib_deps =
  https://github.com/LeeorNahum/BTErrRep.git
```

### 3. First Flash Setup

When enabling core dump for the first time, you must erase the flash:

```bash
pio run --target erase
pio run --target upload
```

### 4. SDK Configuration Requirements

Your project needs these core dump settings enabled (usually in `sdkconfig.esp32-s3-devkitm-1`):

```
CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y
CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF=y
CONFIG_ESP_COREDUMP_CHECKSUM_CRC32=y
CONFIG_ESP_COREDUMP_CHECK_BOOT=y
CONFIG_ESP_COREDUMP_ENABLE=y
```

## Usage

### Normal Boot (No Previous Crash)

```
BTErrRep Crash Dump Example
This demonstrates ESP32 core dump analysis and crash reporting
I (6911) esp_core_dump_flash: Found partition 'coredump' @ 110000 65536 bytes
No previous crash detected - system startup normal
Main service and Crash Dump Service started
Advertising started
To test crash detection, use Serial Monitor commands:
  'crash' - trigger a panic crash
  'assert' - trigger assertion failure
  'wdt' - trigger watchdog timeout
```

### Testing Crash Detection

Use the Serial Monitor to send commands:

- **`crash`** - Triggers `abort()` panic
- **`assert`** - Triggers assertion failure  
- **`wdt`** - Triggers watchdog timeout

### After Crash (Next Boot)

```
BTErrRep Crash Dump Example
This demonstrates ESP32 core dump analysis and crash reporting
Previous crash detected:
  Reason: abort() was called at PC 0x400d66b9 on core 0
  Task: main, PC: 0x400d66b9
  Crash dump reported and cleared
Main service and Crash Dump Service started
```

## BLE Integration

The crash information is automatically reported via BLE with these error codes:

| Error ID | Description |
|----------|-------------|
| 9001 | Crash with panic reason available |
| 9002 | Core dump found but panic reason unavailable |
| 9003 | Detailed crash info with task and PC |

## Troubleshooting

### "No core dump partition found!"

- Verify `partitions_coredump.csv` is in your project
- Ensure `board_build.partitions = partitions_coredump.csv` in platformio.ini
- Run `pio run --target erase && pio run --target upload`

### Core dump functions not available

- Check your sdkconfig has `CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y`
- Ensure `CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF=y` for detailed analysis

### No crash output on normal boot

This is expected behavior - output only appears when a previous crash is detected.

## Files

- `CrashDumpExample.ino` - Main Arduino sketch
- `ble_crash_dump_service.h` - Header for crash dump service
- `ble_crash_dump_service.cpp` - Implementation of crash dump analysis
- `partitions_coredump.csv` - Partition table with core dump partition
- `README.md` - This file

## Memory Usage

The core dump partition uses 64KB (0x10000 bytes). For applications with many tasks or large stacks, you may need to increase the partition size in `partitions_coredump.csv`.

## References

- [ESP-IDF Core Dump Documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/core_dump.html)
- [BTErrRep Library](https://github.com/LeeorNahum/BTErrRep) 
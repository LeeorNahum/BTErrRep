#include "ble_error_report_service.h"

void checkAndReportCrash(BTErrRep& error_report) {
  // Check ESP32 reset reason
  esp_reset_reason_t reset_reason = esp_reset_reason();
  
  switch (reset_reason) {
    case ESP_RST_UNKNOWN:
      Serial.println("Previous reset: Unknown reason");
      error_report.reportError(AlertLevel::MILD_ALERT, 1000, "Unknown reset reason");
      break;
      
    case ESP_RST_POWERON:
      Serial.println("Clean power-on reset");
      error_report.reportError(AlertLevel::NO_ALERT, 1013, "System startup normal");
      break;
      
    case ESP_RST_EXT:
      Serial.println("External reset");
      error_report.reportError(AlertLevel::NO_ALERT, 1014, "External reset detected");
      break;
      
    case ESP_RST_SW:
      Serial.println("Software reset");
      error_report.reportError(AlertLevel::MILD_ALERT, 1006, "Software reset detected");
      break;
      
    case ESP_RST_PANIC:
      Serial.println("Previous reset: Panic/Exception");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1001, "System panic detected");
      break;
      
    case ESP_RST_INT_WDT:
      Serial.println("Previous reset: Interrupt watchdog");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1002, "Interrupt watchdog timeout");
      break;
      
    case ESP_RST_TASK_WDT:
      Serial.println("Previous reset: Task watchdog");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1003, "Task watchdog timeout");
      break;
      
    case ESP_RST_WDT:
      Serial.println("Previous reset: Other watchdog");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1004, "Watchdog reset detected");
      break;
      
    case ESP_RST_DEEPSLEEP:
      Serial.println("Previous reset: Deep sleep wake");
      error_report.reportError(AlertLevel::NO_ALERT, 1015, "Wake from deep sleep");
      break;
      
    case ESP_RST_BROWNOUT:
      Serial.println("Previous reset: Brownout");
      error_report.reportError(AlertLevel::MILD_ALERT, 1005, "Brownout reset detected");
      break;
      
    case ESP_RST_SDIO:
      Serial.println("Previous reset: SDIO");
      error_report.reportError(AlertLevel::MILD_ALERT, 1008, "SDIO reset detected");
      break;
      
    case ESP_RST_USB:
      Serial.println("Previous reset: USB peripheral");
      error_report.reportError(AlertLevel::MILD_ALERT, 1009, "USB reset detected");
      break;
      
    case ESP_RST_JTAG:
      Serial.println("Previous reset: JTAG");
      error_report.reportError(AlertLevel::NO_ALERT, 1016, "JTAG reset detected");
      break;
      
    case ESP_RST_EFUSE:
      Serial.println("Previous reset: eFuse error");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1010, "eFuse error reset");
      break;
      
    case ESP_RST_PWR_GLITCH:
      Serial.println("Previous reset: Power glitch");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1011, "Power glitch detected");
      break;
      
    case ESP_RST_CPU_LOCKUP:
      Serial.println("Previous reset: CPU lockup");
      error_report.reportError(AlertLevel::HIGH_ALERT, 1012, "CPU lockup detected");
      break;
      
    default:
      Serial.println("Unknown reset reason");
      error_report.reportError(AlertLevel::MILD_ALERT, 1007, "Unhandled reset reason");
      break;
  }
}

void bleStartErrorReportService() {
  BTErrRep error_report = BTErrRep(NimBLEDevice::getServer());
  
  error_report.startService();
  
  // Check for previous crash and report it
  checkAndReportCrash(error_report);
}
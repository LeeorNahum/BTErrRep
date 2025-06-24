#include "ble_crash_dump_service.h"

void checkAndReportCrashDump(BTErrRep& error_report) {
  // Initialize core dump system
  esp_core_dump_init();
  
  // Check for previous crash dump
  esp_err_t check = esp_core_dump_image_check();
  if (check == ESP_OK) {
    Serial.println("Previous crash detected:");
    
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH && CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF
    // Get panic reason
    char panic_reason[200];
    esp_err_t err = esp_core_dump_get_panic_reason(panic_reason, sizeof(panic_reason));
    if (err == ESP_OK) {
      Serial.printf("  Reason: %s\n", panic_reason);
      
      // Report crash via BLE with panic reason as emergency text
      error_report.reportError(AlertLevel::HIGH_ALERT, 9001, panic_reason);
    } else {
      Serial.println("  Reason: Core dump found but panic reason unavailable");
      error_report.reportError(AlertLevel::HIGH_ALERT, 9002, "Core dump found but panic reason unavailable");
    }

    // Get task summary
    esp_core_dump_summary_t *summary = (esp_core_dump_summary_t *)malloc(sizeof(esp_core_dump_summary_t));
    if (summary) {
      if (esp_core_dump_get_summary(summary) == ESP_OK) {
        Serial.printf("  Task: %s, PC: 0x%08x\n", summary->exc_task, summary->exc_pc);
        
        // Create detailed crash info string
        char crash_details[300];
        if (err == ESP_OK) {
          snprintf(crash_details, sizeof(crash_details), "Task:%s PC:0x%08x | %s", 
                   summary->exc_task, summary->exc_pc, panic_reason);
        } else {
          snprintf(crash_details, sizeof(crash_details), "Task:%s PC:0x%08x | Core dump analysis", 
                   summary->exc_task, summary->exc_pc);
        }
        
        // Update the error report with more detailed information
        error_report.reportError(AlertLevel::HIGH_ALERT, 9003, crash_details);
      }
      free(summary);
    }
#else
    // Fallback when core dump detailed analysis is not available
    Serial.println("  Core dump found but detailed analysis not configured");
    error_report.reportError(AlertLevel::HIGH_ALERT, 9000, "Previous crash detected - configure ELF format for details");
#endif

    // Clean up the dump after reporting
    esp_core_dump_image_erase();
    Serial.println("  Crash dump reported and cleared");
  } else {
    // No previous crash found - this is normal and expected
    Serial.println("No previous crash detected - system startup normal");
  }
}

void bleStartCrashDumpService() {
  BTErrRep error_report = BTErrRep(NimBLEDevice::getServer());
  
  error_report.startService();
  
  // Check for previous crash dump and report it
  checkAndReportCrashDump(error_report);
} 
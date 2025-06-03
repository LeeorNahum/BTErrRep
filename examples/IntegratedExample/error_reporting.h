#ifndef BLE_ERROR_REPORTING_SERVICE_H
#define BLE_ERROR_REPORTING_SERVICE_H

#include <BTErrRep.h>
#include <esp_system.h>

void bleStartErrorReportingService();
void checkAndReportCrash(BTErrRep& error_report);

// Legacy function - kept for backward compatibility
void setupErrorReporting(NimBLEServer* server);

#endif // BLE_ERROR_REPORTING_SERVICE_H 
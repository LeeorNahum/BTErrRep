#ifndef BLE_ERROR_REPORT_SERVICE_H
#define BLE_ERROR_REPORT_SERVICE_H

#include <BTErrRep.h>
#include <esp_system.h>

void checkAndReportCrash(BTErrRep& error_report);
void bleStartErrorReportService();

#endif // BLE_ERROR_REPORT_SERVICE_H 
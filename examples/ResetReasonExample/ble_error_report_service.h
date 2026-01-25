#ifndef BLE_ERROR_REPORT_SERVICE_H
#define BLE_ERROR_REPORT_SERVICE_H

#include <BTErrRep.h>
#include <esp_system.h>

void checkAndReportCrash(BTErrRepClass& error_report);
void bleStartErrorReportService();

#endif // BLE_ERROR_REPORT_SERVICE_H 
#ifndef BLE_CRASH_DUMP_SERVICE_H
#define BLE_CRASH_DUMP_SERVICE_H

#include <BTErrRep.h>
#include <esp_system.h>
#include <esp_core_dump.h>

void checkAndReportCrashDump(BTErrRep& error_report);
void bleStartCrashDumpService();

#endif // BLE_CRASH_DUMP_SERVICE_H 
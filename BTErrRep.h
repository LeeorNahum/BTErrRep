#ifndef BTERRREP_H
#define BTERRREP_H

#include <NimBLEDevice.h>
#include <Arduino.h>
#include <string>

const NimBLEUUID ALERT_NOTIFICATION_SERVICE_UUID("1811");

const NimBLEUUID ALERT_LEVEL_CHARACTERISTIC_UUID("2A06");
const NimBLEUUID EMERGENCY_ID_CHARACTERISTIC_UUID("2B2D");
const NimBLEUUID EMERGENCY_TEXT_CHARACTERISTIC_UUID("2B2E");
const NimBLEUUID DEVICE_TIME_CHARACTERISTIC_UUID("2B90");

/**
 * @brief Alert levels as defined by BLE Alert Notification Service specification https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/IAS_v1.0/out/en/index-en.html#UUID-619d7932-d68b-1e55-e374-58f74955912f:~:text=4.1.%20Writing%20Alert%20Level%20Behavior
 * @note These values correspond to the standard alert levels for the Alert Level characteristic
 */
enum class AlertLevel : uint8_t {
  NO_ALERT = 0x00,     ///< No alerting shall be done on this device
  MILD_ALERT = 0x01,   ///< The device shall alert (implementation specific)
  HIGH_ALERT = 0x02    ///< The device shall alert in the strongest possible way
};

/**
 * @brief Bluetooth Error Report Service class
 * @note This class implements a Bluetooth error reporting service which provides error information including alert level, emergency ID, emergency text, and device time.
 * @note Alert levels: No Alert, Mild Alert, High Alert
 */
class BTErrRep {
  public:
    BTErrRep(NimBLEServer* server);
    bool startService();
    
    bool reportError(AlertLevel alert_level, uint16_t emergency_id = 0, const std::string& emergency_text = "", uint32_t timestamp = millis());
    
    NimBLEService* getErrorReportService() { return error_report_service; }
    
    NimBLECharacteristic* getAlertLevelCharacteristic() { return alert_level_characteristic; }
    NimBLECharacteristic* getEmergencyIDCharacteristic() { return emergency_id_characteristic; }
    NimBLECharacteristic* getEmergencyTextCharacteristic() { return emergency_text_characteristic; }
    NimBLECharacteristic* getDeviceTimeCharacteristic() { return device_time_characteristic; }
    
  private:
    NimBLEService* error_report_service;
    
    NimBLECharacteristic* alert_level_characteristic;
    NimBLECharacteristic* emergency_id_characteristic;
    NimBLECharacteristic* emergency_text_characteristic;
    NimBLECharacteristic* device_time_characteristic;
    
    void setupDescriptors(NimBLECharacteristic* characteristic, const char* user_description, uint8_t format, int8_t exponent, uint16_t unit, uint8_t namespace_value, uint16_t description);
    
    void createAlertLevelCharacteristic();
    void createEmergencyIDCharacteristic();
    void createEmergencyTextCharacteristic();
    void createDeviceTimeCharacteristic();
};

#endif // BTERRREP_H 
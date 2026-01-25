#ifndef BTERRREP_H
#define BTERRREP_H

#include <NimBLEDevice.h>
#include <Arduino.h>
#include <string>

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
class BTErrRepClass {
  public:
    static const NimBLEUUID SERVICE_UUID;

    static const NimBLEUUID ALERT_LEVEL_CHARACTERISTIC_UUID;
    static const NimBLEUUID EMERGENCY_ID_CHARACTERISTIC_UUID;
    static const NimBLEUUID EMERGENCY_TEXT_CHARACTERISTIC_UUID;
    static const NimBLEUUID DEVICE_TIME_CHARACTERISTIC_UUID;

    /**
     * @brief Start the Error Report service
     * @return true if the service was started successfully, false otherwise
     * @note This service exposes error information including alert level, emergency ID, emergency text, and device time.
     */
    bool startService();
    
    /**
     * @brief Report an error with alert level, emergency ID, emergency text, and timestamp
     * @param alert_level Alert level (NO_ALERT, MILD_ALERT, or HIGH_ALERT)
     * @param emergency_id Optional emergency identifier (default: 0)
     * @param emergency_text Optional emergency description text (default: empty string)
     * @param timestamp Optional timestamp (default: millis(), will use current millis if not specified)
     * @return true if successful, false if service doesn't exist
     * @note This method atomically updates all error characteristics and sends notification for alert level
     */
    bool reportError(AlertLevel alert_level, uint16_t emergency_id = 0, const std::string& emergency_text = "", uint32_t timestamp = millis());
    
    NimBLEService* getService() { return error_report_service; }
    
    NimBLECharacteristic* getAlertLevelCharacteristic() { return alert_level_characteristic; }
    NimBLECharacteristic* getEmergencyIDCharacteristic() { return emergency_id_characteristic; }
    NimBLECharacteristic* getEmergencyTextCharacteristic() { return emergency_text_characteristic; }
    NimBLECharacteristic* getDeviceTimeCharacteristic() { return device_time_characteristic; }
    
  private:
    NimBLEService* error_report_service = nullptr;
    
    NimBLECharacteristic* alert_level_characteristic = nullptr;
    NimBLECharacteristic* emergency_id_characteristic = nullptr;
    NimBLECharacteristic* emergency_text_characteristic = nullptr;
    NimBLECharacteristic* device_time_characteristic = nullptr;
    
    void setupDescriptors(NimBLECharacteristic* characteristic, const char* user_description, uint8_t format, int8_t exponent, uint16_t unit, uint8_t namespace_value, uint16_t description);
    
    void createAlertLevelCharacteristic();
    void createEmergencyIDCharacteristic();
    void createEmergencyTextCharacteristic();
    void createDeviceTimeCharacteristic();
};

extern BTErrRepClass BTErrRep;

#endif // BTERRREP_H
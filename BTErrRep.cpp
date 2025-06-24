#include "BTErrRep.h"

/**
 * @brief Construct a BTErrRep instance
 * @param server A pointer to the server instance this Error Report Service will use
 * @note This service exposes error information including alert level, emergency ID, emergency text, and device time.
 */
BTErrRep::BTErrRep(NimBLEServer* server)
    : error_report_service(nullptr),
      alert_level_characteristic(nullptr),
      emergency_id_characteristic(nullptr),
      emergency_text_characteristic(nullptr),
      device_time_characteristic(nullptr) {
  if (server != nullptr) {
    error_report_service = server->createService(ALERT_NOTIFICATION_SERVICE_UUID);
  }
}

/**
 * @brief Start the Error Report service
 * @return true if the service was started successfully, false otherwise
 */
bool BTErrRep::startService() {
  if (error_report_service) {
    createAlertLevelCharacteristic();
    createEmergencyIDCharacteristic();
    createEmergencyTextCharacteristic();
    createDeviceTimeCharacteristic();

    return error_report_service->start();
  }
  return false;
}

void BTErrRep::setupDescriptors(NimBLECharacteristic* characteristic, const char* user_description, uint8_t format, int8_t exponent, uint16_t unit, uint8_t namespace_value, uint16_t description) {
  if (characteristic == nullptr) return;
  
  // User Description Descriptor
  NimBLEDescriptor* characteristic_user_description_descriptor = characteristic->createDescriptor(NimBLEUUID("2901"), NIMBLE_PROPERTY::READ);
  characteristic_user_description_descriptor->setValue(user_description);
  
  // Presentation Format Descriptor
  NimBLE2904* characteristic_presentation_format_descriptor = (NimBLE2904*)characteristic->createDescriptor(NimBLEUUID("2904"));
  
  characteristic_presentation_format_descriptor->setFormat(format);
  characteristic_presentation_format_descriptor->setExponent(exponent);
  characteristic_presentation_format_descriptor->setUnit(unit);
  characteristic_presentation_format_descriptor->setNamespace(namespace_value);
  characteristic_presentation_format_descriptor->setDescription(description);
}

void BTErrRep::createAlertLevelCharacteristic() {
  if (error_report_service == nullptr || alert_level_characteristic != nullptr) return;

  alert_level_characteristic = error_report_service->createCharacteristic(
    ALERT_LEVEL_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );
  setupDescriptors(alert_level_characteristic, "Alert Level", NimBLE2904::FORMAT_UINT8, 0x00, 0x2700, 0x01, 0x0000);
}

void BTErrRep::createEmergencyIDCharacteristic() {
  if (error_report_service == nullptr || emergency_id_characteristic != nullptr) return;

  emergency_id_characteristic = error_report_service->createCharacteristic(
    EMERGENCY_ID_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ
  );
  setupDescriptors(emergency_id_characteristic, "Emergency ID", NimBLE2904::FORMAT_UINT16, 0x00, 0x2700, 0x01, 0x0000);
}

void BTErrRep::createEmergencyTextCharacteristic() {
  if (error_report_service == nullptr || emergency_text_characteristic != nullptr) return;

  emergency_text_characteristic = error_report_service->createCharacteristic(
    EMERGENCY_TEXT_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ
  );
  setupDescriptors(emergency_text_characteristic, "Emergency Text", NimBLE2904::FORMAT_UTF8, 0x00, 0x2700, 0x01, 0x0000);
}

void BTErrRep::createDeviceTimeCharacteristic() {
  if (error_report_service == nullptr || device_time_characteristic != nullptr) return;

  device_time_characteristic = error_report_service->createCharacteristic(
    DEVICE_TIME_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ
  );
  setupDescriptors(device_time_characteristic, "Device Time", NimBLE2904::FORMAT_UINT32, 0x00, 0x2700, 0x01, 0x0000);
}

/**
 * @brief Report an error with alert level, emergency ID, emergency text, and timestamp
 * @param alert_level Alert level (NO_ALERT, MILD_ALERT, or HIGH_ALERT)
 * @param emergency_id Optional emergency identifier (default: 0)
 * @param emergency_text Optional emergency description text (default: empty string)
 * @param timestamp Optional timestamp (default: millis(), will use current millis if not specified)
 * @return true if successful, false if service doesn't exist
 * @note This method atomically updates all error characteristics and sends notification for alert level
 */
bool BTErrRep::reportError(AlertLevel alert_level, uint16_t emergency_id, const std::string& emergency_text, uint32_t timestamp) {
  if (error_report_service == nullptr) return false;
  if (alert_level_characteristic == nullptr || emergency_id_characteristic == nullptr || emergency_text_characteristic == nullptr || device_time_characteristic == nullptr) return false;
  
  alert_level_characteristic->setValue(static_cast<uint8_t>(alert_level));
  emergency_id_characteristic->setValue(emergency_id);
  emergency_text_characteristic->setValue(emergency_text);
  device_time_characteristic->setValue(timestamp);
  
  alert_level_characteristic->notify();
  
  return true;
} 
#include "BTErrRep.h"

const NimBLEUUID BTErrRepClass::SERVICE_UUID("1811");

const NimBLEUUID BTErrRepClass::ALERT_LEVEL_CHARACTERISTIC_UUID("2A06");
const NimBLEUUID BTErrRepClass::EMERGENCY_ID_CHARACTERISTIC_UUID("2B2D");
const NimBLEUUID BTErrRepClass::EMERGENCY_TEXT_CHARACTERISTIC_UUID("2B2E");
const NimBLEUUID BTErrRepClass::DEVICE_TIME_CHARACTERISTIC_UUID("2B90");

BTErrRepClass BTErrRep;

bool BTErrRepClass::startService() {
  NimBLEServer* pServer = NimBLEDevice::getServer();
  if (pServer == nullptr) return false;

  error_report_service = pServer->getServiceByUUID(SERVICE_UUID);
  if (error_report_service == nullptr) {
    error_report_service = pServer->createService(SERVICE_UUID);
  }

  createAlertLevelCharacteristic();
  createEmergencyIDCharacteristic();
  createEmergencyTextCharacteristic();
  createDeviceTimeCharacteristic();

  return error_report_service->start();
}

void BTErrRepClass::setupDescriptors(NimBLECharacteristic* characteristic, const char* description_text, uint8_t format, int8_t exponent, uint16_t unit, uint8_t namespace_value, uint16_t description_value) {
  if (characteristic == nullptr) return;
  
  NimBLEDescriptor* user_description = characteristic->createDescriptor(NimBLEUUID("2901"), NIMBLE_PROPERTY::READ);
  user_description->setValue(description_text);
  
  NimBLE2904* presentation_format = (NimBLE2904*)characteristic->createDescriptor(NimBLEUUID("2904"), NIMBLE_PROPERTY::READ);
  presentation_format->setFormat(format);
  presentation_format->setExponent(exponent);
  presentation_format->setUnit(unit);
  presentation_format->setNamespace(namespace_value);
  presentation_format->setDescription(description_value);
}

void BTErrRepClass::createAlertLevelCharacteristic() {
  if (error_report_service == nullptr) return;

  if (alert_level_characteristic == nullptr) {
    alert_level_characteristic = error_report_service->getCharacteristic(ALERT_LEVEL_CHARACTERISTIC_UUID);
    if (alert_level_characteristic == nullptr) {
      alert_level_characteristic = error_report_service->createCharacteristic(
        ALERT_LEVEL_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
      );
      setupDescriptors(alert_level_characteristic, "Alert Level", NimBLE2904::FORMAT_UINT8, 0x00, 0x2700, 0x00, 0x0000);
    }
  }
}

void BTErrRepClass::createEmergencyIDCharacteristic() {
  if (error_report_service == nullptr) return;

  if (emergency_id_characteristic == nullptr) {
    emergency_id_characteristic = error_report_service->getCharacteristic(EMERGENCY_ID_CHARACTERISTIC_UUID);
    if (emergency_id_characteristic == nullptr) {
      emergency_id_characteristic = error_report_service->createCharacteristic(
        EMERGENCY_ID_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ
      );
      setupDescriptors(emergency_id_characteristic, "Emergency ID", NimBLE2904::FORMAT_UINT16, 0x00, 0x2700, 0x00, 0x0000);
    }
  }
}

void BTErrRepClass::createEmergencyTextCharacteristic() {
  if (error_report_service == nullptr) return;

  if (emergency_text_characteristic == nullptr) {
    emergency_text_characteristic = error_report_service->getCharacteristic(EMERGENCY_TEXT_CHARACTERISTIC_UUID);
    if (emergency_text_characteristic == nullptr) {
      emergency_text_characteristic = error_report_service->createCharacteristic(
        EMERGENCY_TEXT_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ
      );
      setupDescriptors(emergency_text_characteristic, "Emergency Text", NimBLE2904::FORMAT_UTF8, 0x00, 0x2700, 0x00, 0x0000);
    }
  }
}

void BTErrRepClass::createDeviceTimeCharacteristic() {
  if (error_report_service == nullptr) return;

  if (device_time_characteristic == nullptr) {
    device_time_characteristic = error_report_service->getCharacteristic(DEVICE_TIME_CHARACTERISTIC_UUID);
    if (device_time_characteristic == nullptr) {
      device_time_characteristic = error_report_service->createCharacteristic(
        DEVICE_TIME_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ
      );
      setupDescriptors(device_time_characteristic, "Device Time", NimBLE2904::FORMAT_UINT32, 0x00, 0x2700, 0x00, 0x0000);
    }
  }
}

bool BTErrRepClass::reportError(AlertLevel alert_level, uint16_t emergency_id, const std::string& emergency_text, uint32_t timestamp) {
  if (error_report_service == nullptr) return false;
  if (alert_level_characteristic == nullptr || emergency_id_characteristic == nullptr || emergency_text_characteristic == nullptr || device_time_characteristic == nullptr) return false;
  
  alert_level_characteristic->setValue(static_cast<uint8_t>(alert_level));
  emergency_id_characteristic->setValue(emergency_id);
  emergency_text_characteristic->setValue(emergency_text);
  device_time_characteristic->setValue(timestamp);
  
  alert_level_characteristic->notify();
  
  return true;
} 
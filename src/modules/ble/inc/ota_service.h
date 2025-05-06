#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H

// OTA包大小
#define OTA_PACKET_SIZE             20

// Service UUID
#define OTA_SERVICE_UUID            0xFFE0

// Characteristic UUIDs
#define OTA_UUID                    0xFFE1

// GATT函数
void addGattOtaService(void);

#endif
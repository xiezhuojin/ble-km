#ifndef HID_SERVICE_H
#define HID_SERVICE_H

// Service UUID
#define HID_SERVICE_UUID            0xFFF0

// Characteristic UUIDs
#define KEYBOARD_REPORT_UUID        0xFFF1
#define MOUSE_REPORT_UUID           0xFFF2

// GATT函数
void addGattHidService();

#endif
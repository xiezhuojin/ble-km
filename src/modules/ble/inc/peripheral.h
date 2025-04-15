#ifndef PERIPHERIAL_H
#define PERIPHERIAL_H

// Simple BLE Peripheral Task Events
#define SBP_START_DEVICE_EVT    0x0001

// 蓝牙设备
#define PERIPHERIAL_MAGIC       'b', 'l', 'e', '-', 'k', 'm'
#define PERIPHERIAL_MAGIC_SIZE  7
#define DEVICE_NAME             "ble-km"

// 蓝牙函数
void initBlePeripheral();
void startBlePeripheral();

#endif
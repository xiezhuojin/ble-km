#ifndef OTA_H
#define OTA_H

#include "stdint.h"
#include "stdbool.h"

// OTA数据包
typedef struct {
    uint8_t cmd;                    // 命令
    uint8_t length;                 // 数据长度
    uint8_t data[];                 // 数据
} OtaPacket;

// OTA函数
bool handleOta(OtaPacket*);

#endif
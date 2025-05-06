#ifndef OTA_H
#define OTA_H

#include "stdint.h"
#include "stdbool.h"

/* 整个用户code区分成四块，4K，216K，216K，12K，后三块下面分别叫做imageA（APP），imageB（OTA）和imageIAP */

/* FLASH定义 */
#define FLASH_BLOCK_SIZE       EEPROM_BLOCK_SIZE
#define IMAGE_SIZE             216 * 1024

/* imageA定义 */
#define IMAGE_A_START_ADD      4 * 1024
#define IMAGE_A_SIZE           IMAGE_SIZE

/* imageB定义 */
#define IMAGE_B_START_ADD      (IMAGE_A_START_ADD + IMAGE_SIZE)
#define IMAGE_B_SIZE           IMAGE_SIZE

/* imageIAP定义 */
#define IMAGE_IAP_FLAG         0x03
#define IMAGE_IAP_START_ADD    (IMAGE_B_START_ADD + IMAGE_SIZE)
#define IMAGE_IAP_SIZE         12 * 1024

/* 存放在DataFlash地址，不能占用蓝牙的位置 */
#define OTA_DATAFLASH_ADD      0x00077000 - FLASH_ROM_MAX_SIZE

// 命令
#define CMD_START               0x01  // 开始升级
#define CMD_DATA                0x02  // 数据传输
#define CMD_END                 0x03  // 结束升级

// OTA数据包
typedef struct {
    uint8_t cmd;                // 命令
    uint8_t length;             // 数据长度
    uint8_t data[];             // 数据
} OtaPacket;

// OTA函数
bool handleOta(OtaPacket*);

#endif
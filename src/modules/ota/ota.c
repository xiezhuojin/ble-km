#include "ota.h"
#include "stdbool.h"
#include "config.h"


/* flash的数据临时存储 */
__attribute__((aligned(8))) uint8_t block_buf[16];

/*********************************************************************
 * @fn      DisableAllIRQ
 *
 * @brief   关闭所有的中断
 *
 * @return  none
 */
void DisableAllIRQ(void)
{
    SYS_DisableAllIrq(NULL);
}

/*********************************************************************
 * @fn      SwitchImageFlag
 *
 * @brief   切换dataflash里的ImageFlag
 *
 * @param   new_flag    - 切换的ImageFlag
 *
 * @return  none
 */
void SwitchImageFlag(uint8_t new_flag)
{
    /* 读取第一块 */
    EEPROM_READ(OTA_DATAFLASH_ADD, (uint32_t *)&block_buf[0], 4);

    /* 擦除第一块 */
    EEPROM_ERASE(OTA_DATAFLASH_ADD, EEPROM_PAGE_SIZE);

    /* 更新Image信息 */
    block_buf[0] = new_flag;

    /* 编程DataFlash */
    EEPROM_WRITE(OTA_DATAFLASH_ADD, (uint32_t *)&block_buf[0], 4);
}

/*********************************************************************
 * @fn      handleOta
 *
 * @brief   处理OTA数据
 *
 * @param   packet - pointer to OtaPacket
 *
 * @return  none
 */
bool handleOta(OtaPacket *packet) {
    const uint32_t startAddr = IMAGE_B_START_ADD;
    static uint32_t writeAddr = startAddr;

    OtaPacket *data = (OtaPacket *) packet;
    bool result = true;
    switch (data->cmd) {
        case CMD_START:
            result = FLASH_ROM_ERASE(startAddr, IMAGE_B_SIZE);
            writeAddr = startAddr;
            break;
        case CMD_DATA:
            result = FLASH_ROM_WRITE(writeAddr, data->data, data->length);
            writeAddr += data->length;
            break;
        case CMD_END:
            /* 当前的是ImageA */
            /* 关闭当前所有使用中断，或者方便一点直接全部关闭 */
            DisableAllIRQ();

            /* 修改DataFlash，切换至ImageIAP */
            SwitchImageFlag(IMAGE_IAP_FLAG);

            /* 等待打印完成 ，复位*/
            mDelaymS(10);
            SYS_ResetExecute();
        default:
            result = false;
    }
    return result;
}
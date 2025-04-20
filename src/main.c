#include "CH57x_common.h"
#include "config.h"
#include "hid.h"
#include "peripheral.h"
#include "devinfo_service.h"
#include "hid_service.h"

extern void USB_IRQHandler(void);

void sendKey(uint8_t key) {
    KeyboardReport report = {
        .modifiers = 0x0,
        .reserved = 0x0,
        .keys = {},
    };
    report.keys[0] = key;
    sendKeyboardReport(&report);
}


/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   主循环
 *
 * @return  none
 */
__attribute__((section(".highcode")))
__attribute__((noinline))
void Main_Circulation(void)
{
    while(1)
    {
        TMOS_SystemProcess();
    }
}

int main() {
    // 设定系统时钟
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    // 设置LED
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_5mA);
    GPIOA_SetBits(GPIO_Pin_8);

    // init hid
    initHid();

    // 初始化蓝牙外设
    initBlePeripheral();
    // 添加GATT服务
    addGattDevinfoService();
    addGattHidService();
    // 开始蓝牙外设服务
    startBlePeripheral();

    Main_Circulation();

    // while (1) {
    //     GPIOA_InverseBits(GPIO_Pin_8);
    //     mDelaymS(2000);

    //     const unsigned char keyCodes[] = {
    //         0x0B,  // H
    //         0x08,  // e
    //         0x0F,  // l
    //         0x0F,  // l
    //         0x12,  // o
    //         0x2C,  // (空格)
    //         0x1A,  // W
    //         0x12,  // o
    //         0x15,  // r
    //         0x0F,  // l
    //         0x07,  // d
    //         0x2C,  // (空格)
    //     };
    //     for (int i = 0; i < sizeof(keyCodes); i++) {
    //         sendKey(keyCodes[i]);
    //         sendKey(0x00);
    //     }
    // }
}
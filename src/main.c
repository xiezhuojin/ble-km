#include "CH57x_common.h"
#include "config.h"
#include "peripheral.h"
#include "hid.h"
#include "devinfo_service.h"
#include "hid_service.h"
#include "ota_service.h"

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

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main() {
    // 系统主频
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    // 串口初始化，蓝牙外设使用
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();

    // Hid初始化，以供Hid服务使用
    initHid();

    // 初始化蓝牙外设
    initBlePeripheral();

    // 添加GATT服务
    addGattDevinfoService();
    addGattHidService();
    addGattOtaService();

    Main_Circulation();
}
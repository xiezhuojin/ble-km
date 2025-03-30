#include "CH57x_common.h"
#include "hid.h"

// extern void USB_IRQHandler(void);

int main() {
    // 设定系统时钟
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    // 设置USB
    USB_DeviceInit();
    PFIC_EnableIRQ(USB_IRQn);

    // // 发送hello world
    // KeyboardReport report_1;

    while (1) {
        
    }
}
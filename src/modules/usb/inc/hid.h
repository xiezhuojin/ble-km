#ifndef HID_H
#define HID_H

#include <stdint.h>

// USB端点大小
#define DevEP0SIZE 0x40

// 发送超时时间
#define timeoutMs 10

// 键盘报告
typedef struct {
    uint8_t modifiers; // 修饰键（Ctrl, Shift等）
    uint8_t reserved;  // 保留字节
    uint8_t keys[6];   // 最多6个按键的同时按下
} KeyboardReport;

// 鼠标报告
typedef struct {
    uint8_t buttons; // 按钮状态
    uint8_t x;       // X轴移动
    uint8_t y;       // Y轴移动
    uint8_t wheel;   // 滚轮移动
} MouseReport;

// USB中断
void USB_IRQHandler(void);

// hid函数
void initHid();
void sendKeyboardReport(KeyboardReport*);
void sendMouseReport(MouseReport*);

#endif
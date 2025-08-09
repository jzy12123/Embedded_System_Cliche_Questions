#include <stdio.h>
#include <stdint.h>
#include <windows.h>

// 模拟8位控制寄存器
typedef union
{
    uint8_t value; // 完整寄存器值
    struct
    {
        uint8_t enable : 1;   // bit0: 使能位
        uint8_t mode : 2;     // bit1-2: 模式选择
        uint8_t reserved : 3; // bit3-5: 保留位
        uint8_t status : 2;   // bit6-7: 状态位
    } bits;
} ControlRegister;

int main()
{
    SetConsoleOutputCP(65001); // 设置控制台为UTF-8编码
    ControlRegister reg = {0};

    // 设置寄存器值
    reg.value = 0xC5; // 二进制: 11000101

    // 通过位域访问各个字段
    printf("完整寄存器值: 0x%X\n", reg.value);
    printf("使能位: %d\n", reg.bits.enable); // 1
    printf("模式选择: %d\n", reg.bits.mode); // 2 (01)
    printf("状态位: %d\n", reg.bits.status); // 3 (11)

    // 修改位域值
    reg.bits.mode = 3; // 修改模式为最大值

    printf("修改后的寄存器值: 0x%X\n", reg.value); // 0xC7

    return 0;
}

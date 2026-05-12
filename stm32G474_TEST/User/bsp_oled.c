/**
 * @file bsp_oled.c
 * @brief 硬件I2C驱动OLED
 * @author chy
 * @version 1.0.0
 * @date 2026-03-25
 */

#include "bsp_oled.h"
#include "oled_Front.h"
#include "main.h"  
#include "software_I2C.h"

//	/* 硬件I2C句柄（由CubeMX自动生成） */
//	extern I2C_HandleTypeDef hi2c1;

/* OLED I2C地址 */
#define OLED_I2C_ADDR    0x78

/*============================ 硬件I2C 写命令 ================================*/
// 【必须定义】0.96寸I2C OLED标准地址，固定为0x3C<<1=0x78

// 【必须定义】OLED控制字节：0x00=后续是命令，0x40=后续是数据
#define OLED_CMD_CTRL  0x00
#define OLED_DATA_CTRL 0x40

// 【完全重写】OLED写命令函数（符合I2C协议完整帧）
void OLED_WriteCommand(uint8_t Cmd)
{
    MyI2C_Start();                          // 1. 发送I2C起始信号
    MyI2C_SendByte(OLED_I2C_ADDR);         // 2. 发送OLED从机地址+写方向
    MyI2C_ReceiveAck();                     // 3. 等待从机应答
    MyI2C_SendByte(OLED_CMD_CTRL);         // 4. 发送控制字节：后续是命令
    MyI2C_ReceiveAck();                     // 5. 等待从机应答
    MyI2C_SendByte(Cmd);                    // 6. 发送要写入的命令
    MyI2C_ReceiveAck();                     // 7. 等待从机应答
    MyI2C_Stop();                           // 8. 发送I2C停止信号
}

// 【完全重写】OLED写数据函数（符合I2C协议完整帧）
void OLED_WriteData(uint8_t Byte)
{
    MyI2C_Start();                          // 1. 发送I2C起始信号
    MyI2C_SendByte(OLED_I2C_ADDR);         // 2. 发送OLED从机地址+写方向
    MyI2C_ReceiveAck();                     // 3. 等待从机应答
    MyI2C_SendByte(OLED_DATA_CTRL);        // 4. 发送控制字节：后续是显示数据
    MyI2C_ReceiveAck();                     // 5. 等待从机应答
    MyI2C_SendByte(Byte);                   // 6. 发送要写入的显示数据
    MyI2C_ReceiveAck();                     // 7. 等待从机应答
    MyI2C_Stop();                           // 8. 发送I2C停止信号
}

/*============================ 设置光标 ====================================*/
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/*============================ 清屏 ========================================*/
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/*============================ 部分清屏 ====================================*/
void OLED_Clear_Part(uint8_t Line, uint8_t start, uint8_t end)
{
    uint8_t i, Column;
    for (Column = start; Column <= end; Column++)
    {
        OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(0x00);
        }
        OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/*============================ 显示字符 ====================================*/
// 【注意】必须提前定义好OLED_F8x16字库数组，否则无法显示
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
    }
}

/*============================ 显示字符串 ==================================*/
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/*============================ 显示中文单字 ================================*/
// 【注意】必须提前定义好OLED_F16x16中文字库数组，否则无法显示
void OLED_ShowWord(uint8_t Line, uint8_t Column, uint8_t Chinese)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F16x16[Chinese][i]);
    }
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8 + 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F16x16[Chinese][i + 8]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F16x16[Chinese][i + 16]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8 + 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F16x16[Chinese][i + 24]);
    }
}

/*============================ 显示中文串 ==================================*/
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t *Chinese, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowWord(Line, Column + i * 2, Chinese[i]);
    }
}

/*============================ 次方函数 ====================================*/
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/*============================ 显示数字 ====================================*/
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/*============================ 显示有符号数字 ==============================*/
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/*============================ 显示十六进制 ================================*/
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/*============================ 显示二进制 ================================*/
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/*============================ OLED 初始化 ======================*/
void OLED_Init(void)
{
    HAL_Delay(100); // 上电延时，必加！等待OLED上电稳定

    OLED_WriteCommand(0xAE); // 关闭显示
    OLED_WriteCommand(0xD5); // 设置时钟分频因子
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); // 设置驱动路数
    OLED_WriteCommand(0x3F); // 1/64 duty
    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00); // 无偏移
    OLED_WriteCommand(0x40); // 设置显示开始行
    OLED_WriteCommand(0xA1); // 左右方向：0xA1=正常，0xA0=镜像
    OLED_WriteCommand(0xC8); // 上下方向：0xC8=正常，0xC0=颠倒
    OLED_WriteCommand(0xDA); // 设置COM硬件引脚配置
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); // 对比度设置
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4); // 全局显示开启
    OLED_WriteCommand(0xA6); // 显示方式：正常显示
    OLED_WriteCommand(0x8D); // 电荷泵设置
    OLED_WriteCommand(0x14); // 开启电荷泵
    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // 初始化后清屏，避免乱码
} 

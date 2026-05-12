/**
 * @file software_I2C.c
 * @brief »Ìº˛I2C«˝∂Ø£¨–ﬁ∏¥∫Û  ≈‰OLED
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 */

/*============================ INCLUDES ======================================*/
#include "main.h"

/*============================ MACROS ========================================*/
#define I2C_SCL_PIN  GPIO_PIN_13
#define I2C_SDA_PIN  GPIO_PIN_14
#define I2C_PORT     GPIOB
#define I2C_DELAY_VAL  20
#define I2C_Delay_ms(x)  HAL_Delay(x)
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/

static void MyI2C_Delay(void)
{
    uint8_t i = I2C_DELAY_VAL;
    while(i--);
}

void MyI2C_W_SCL(uint8_t BitValue)
{
    HAL_GPIO_WritePin(I2C_PORT, I2C_SCL_PIN, (GPIO_PinState)BitValue);
    MyI2C_Delay();
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    HAL_GPIO_WritePin(I2C_PORT, I2C_SDA_PIN, (GPIO_PinState)BitValue);
    MyI2C_Delay();
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = HAL_GPIO_ReadPin(I2C_PORT, I2C_SDA_PIN);
    MyI2C_Delay();
    return BitValue;
}

void MyI2C_Init(void)
{
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/*============================ EXTERNAL IMPLEMENTATION =======================*/

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i=0; i<8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
   
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;					
	MyI2C_W_SDA(1);							
	for (i = 0; i < 8; i ++)				
	{
		MyI2C_W_SCL(1);					
		if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}	
		MyI2C_W_SCL(0);						
	}
	return Byte;						
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}

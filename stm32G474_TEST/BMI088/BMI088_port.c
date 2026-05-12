/**
 * @file BMI088_port.c
 * @brief 1
 *
 * This file contains implementation of initialization and processing
 * functions for the BMI088_port module.
 *
 * @author 1
 * @version 1.0.0
 * @date 2026-04-26
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-26	1	Initial version
 */

#include "BMI088_port.h"

/*============================ INCLUDES ======================================*/
#include "bmi08x.h"
#include "spi.h"   
#include "gpio.h"
#include "bmi088_mm.h"
#include "bmi08x.h"
#include "bmi08.h"

/*============================ MACROS ========================================*/
#define ACC_CS_PIN    GPIO_PIN_4		// º”ÀŸ∂»
#define ACC_CS_PORT   GPIOC

#define GYRO_CS_PIN   GPIO_PIN_5		// Õ”¬›“«
#define GYRO_CS_PORT  GPIOC
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
extern SPI_HandleTypeDef hspi1;
/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
int8_t bmi08_acc_spi_read(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
    uint8_t txbuf[2] = {reg_addr | 0x80, 0x00};
    uint8_t rxbuf[2] = {0};

    HAL_SPI_TransmitReceive(&hspi1, txbuf, rxbuf, 2, 100);
    HAL_SPI_Receive(&hspi1, data, len, 100);

    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
    return 0;
}

int8_t bmi08_acc_spi_write(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
    uint8_t txbuf[2] = {reg_addr & 0x7F, data[0]};

    HAL_SPI_Transmit(&hspi1, txbuf, 2, 100);

    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
    return 0;
}

int8_t bmi08_gyro_spi_read(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);

    reg_addr |= 0x80;
    HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
    HAL_SPI_Receive(&hspi1, data, len, 100);

    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);
    return 0;
}

int8_t bmi08_gyro_spi_write(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);

    reg_addr &= 0x7F;
    HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
    HAL_SPI_Transmit(&hspi1, data, len, 100);

    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);
    return 0;
}


void bmi08_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/
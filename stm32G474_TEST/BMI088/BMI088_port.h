/**
 * @file BMI088_port.h
 * @brief 1
 *
 * This file contains function declarations and necessary includes
 * for the BMI088_port module.
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

#ifndef BMI088_PORT_H
#define BMI088_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
int8_t bmi08_acc_spi_read(uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t bmi08_acc_spi_write(uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t bmi08_gyro_spi_read(uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t bmi08_gyro_spi_write(uint8_t reg_addr, uint8_t *data, uint16_t len);
void bmi08_delay_ms(uint32_t ms);


#ifdef __cplusplus
}
#endif

#endif // BMI088_PORT_H

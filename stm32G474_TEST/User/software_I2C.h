/**
 * @file software_I2C.h
 * @brief 1211
 *
 * This file contains function declarations and necessary includes
 * for the software_I2C module.
 *
 * @author 2
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	2	Initial version
 */

#ifndef SOFTWARE_I2C_H
#define SOFTWARE_I2C_H

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

static void MyI2C_Delay(void);
void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
uint8_t MyI2C_R_SDA(void);
void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveAck(void);
uint8_t MyI2C_ReceiveByte(void);


#ifdef __cplusplus
}
#endif

#endif // SOFTWARE_I2C_H

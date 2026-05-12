/**
 * @file SHT40.h
 * @brief 11
 *
 * This file contains function declarations and necessary includes
 * for the SHT40 module.
 *
 * @author 40
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	40	Initial version
 */

#ifndef SHT40_H
#define SHT40_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/
#include "string.h"
#include "usart.h"
#include "stm32g4xx_hal.h" 
/*============================ MACROS ========================================*/
#define SHT40_RX_BUF_SIZE    32
/*============================ TYPES =========================================*/
typedef enum {
    SHT40_MODE_MANUAL = 0,  
    SHT40_MODE_AUTO        
} SHT40_ModeTypeDef;

typedef struct {
    float temperature;     
    float humidity;        
    uint8_t data_valid;     
} SHT40_DataTypeDef;

/*============================ GLOBAL VARIABLES ==============================*/
extern SHT40_DataTypeDef sht40_data;
extern uint8_t sht40_rx_buf[SHT40_RX_BUF_SIZE];
extern uint8_t sht40_rx_len;
/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/

void SHT40_Init(SHT40_ModeTypeDef mode);    
void SHT40_SendCmd(const char *cmd);        
uint8_t SHT40_ParseData(uint8_t *buf, uint16_t len); 
void SHT40_UART_RxCpltCallback(UART_HandleTypeDef *huart); 

#ifdef __cplusplus
}
#endif

#endif // SHT40_H

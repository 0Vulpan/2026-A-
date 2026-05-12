/**
 * @file MY_ADC.h
 * @brief QQW
 *
 * This file contains function declarations and necessary includes
 * for the MY_ADC module.
 *
 * @author QW
 * @version 1.0.0
 * @date 2026-04-25
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-25	QW	Initial version
 */

#ifndef MY_ADC_H
#define MY_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "main.h"
/*============================ INCLUDES ======================================*/
#define ADC_CHANNEL_COUNT    3    
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
extern uint16_t adc_buf[ADC_CHANNEL_COUNT];

extern float    voltage_ch0;
extern uint16_t light_ch0;

extern float    current;
extern float    current_mA;

extern uint16_t adc_ch2_value;
extern uint16_t adc_ch2_voltage_mv;
extern uint8_t  water_level_mm;/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void ADC_StartChannel(void);
void ADC_Process_CH0(void);
void ADC_Process_CH1(void);
void ADC_Process_CH2(void);
void ADC_Process_ALL(void);

void ADC_Test(void);

#ifdef __cplusplus
}
#endif

#endif // MY_ADC_H

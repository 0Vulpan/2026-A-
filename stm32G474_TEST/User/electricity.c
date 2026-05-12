/**
 * @file electricity.c
 * @brief 12
 *
 * This file contains implementation of initialization and processing
 * functions for the electricity module.
 *
 * @author Wadadada
 * @version 1.0.0
 * @date 2026-04-25
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-25	12	Initial version
 */

#include "electricity.h"
#include "adc.h"

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/
//#define INA240_GAIN        100.0f    // 你的INA240增益（A3=100）
//#define RSENSE             0.01f     // 采样电阻阻值（单位：Ω，如 0.01Ω=10mΩ）
//#define ADC_REF_VOLTAGE     3.3f      // ADC参考电压（3.3V 或 3.0V）
//#define ADC_RESOLUTION      4095.0f   // 12位ADC最大数值（2^12 -1）

//extern ADC_HandleTypeDef hadc1;
//uint16_t adc_raw_val = 0;
//float ina240_voltage = 0.0f;
//float current = 0.0f;
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
//float INA240_Read_Current(void)
//{
//    HAL_ADC_Start(&hadc1);
//        if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
//    {
//        adc_raw_val = HAL_ADC_GetValue(&hadc1);
//        ina240_voltage = (float)adc_raw_val * ADC_REF_VOLTAGE / ADC_RESOLUTION;
//        
//        // I = Vout / (GAIN * Rsense)
//        current = ina240_voltage / (INA240_GAIN * RSENSE);
//    }
//        HAL_ADC_Stop(&hadc1);
//    
//		float current_mA = current * 1000;
//		LOG_I("I:%.2f A / %.2f mA\r\n", current, current_mA);
//}
/*============================ EXTERNAL IMPLEMENTATION =======================*/


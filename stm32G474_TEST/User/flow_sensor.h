/**
 * @file flow_sensor.h
 * @brief 1
 *
 * This file contains function declarations and necessary includes
 * for the flow_sensor module.
 *
 * @author 1
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	1	Initial version
 */

#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx_hal.h"
#define YFS401_Pin        GPIO_PIN_15
#define YFS401_GPIO_Port  GPIOB
typedef struct
{
    uint8_t receive_flag;// 1秒定时标志位
    uint32_t pluse_1s;// 每秒脉冲
	  float  acculat;//累计流量
    float flow_value;// 最终流量值
	  float   instant;// 瞬时流量 L/min
} golbal_flow_t;
extern golbal_flow_t golbal_flow;
typedef enum {
    MODE_4_PIPE = 0,   // 四分管
    MODE_6_PIPE = 1,   // 六分管
    MODE_6MM_PIPE = 2  // 6mm管
} FlowModel;
void flow_sensor_Init();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Flow_Read(void);






/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/

#ifdef __cplusplus
}

#endif


#endif // FLOW_SENSOR_H

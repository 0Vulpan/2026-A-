/**
 * @file drv_encoder.h
 * @brief 编码器测速为后续PID进行服务
 *
 * This file contains function declarations and necessary includes
 * for the drv_encoder module.
 *
 * @author Xue
 * @version 1.0.0
 * @date 2025-07-03
 *
 * @Copyright (c) 2025, Xue Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2025-07-03	Xue	Initial version
 */

#ifndef DRV_ENCODER_H
#define DRV_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx.h"                  // Device header

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
// 电机方向定义
typedef enum {
    ENCODER_DIR_FORWARD = 0,
    ENCODER_DIR_REVERSE = 1,
		ENCODER_DIR_STOP 		= 3,
} EncoderDirection_t;

// 编码器测速结构体
typedef struct {
    uint32_t last_cnt;                      // 上次计数值
    float motor_rpm;                        // 当前转速(RPM)
    EncoderDirection_t direction;           // 当前方向
    float motor_position_revolutions;       // 累计转动圈数
    uint32_t pulses_per_motor_revolution;   // 一圈对应的脉冲数
    float sample_time_s;                    // 采样周期(s)
} EncoderSpeedHandle_t;
/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void EncoderSpeed_Init(uint16_t pulses_per_rev, uint8_t gear_ratio, uint8_t multiplier, float sample_time_s);
void EncoderSpeed_TimerCallback(void);

float EncoderSpeed_GetRPM(void);
EncoderDirection_t EncoderSpeed_GetDirection(void);
float EncoderSpeed_GetPosition(void);
#ifdef __cplusplus
}
#endif

#endif // DRV_ENCODER_H

/**1
 * @file encoder.h
 * @brief a
 *
 * This file contains function declarations and necessary includes
 * for the encoder module.
 *
 * @author chy
 * @version 1.0.0
 * @date 2026-04-02
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-02	chy	Initial version
 */

#ifndef ENCODER_H
#define ENCODER_H

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
		ENCODER_DIR_STOP = 2,
} EncoderDirection_t;

// 编码器测速结构体
typedef struct {
    uint32_t last_cnt;                      // 上次计数值
	uint32_t last_cnt_right;
    float motor_rpm;                        // 当前转速(RPM)
	float motor_rpm_right;
    EncoderDirection_t direction;           // 当前方向
	EncoderDirection_t direction_right;
    float motor_position_revolutions;       // 累计转动圈数
	float motor_position_revolutions_right;
    uint32_t pulses_per_motor_revolution;   // 一圈对应的脉冲数
	uint32_t pulses_per_motor_revolution_right;
    float sample_time_s;                    // 采样周期(s)
	float sample_time_s_right;
} EncoderSpeedHandle_t;


/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void EncoderSpeed_Init(uint16_t pulses_per_rev, uint8_t gear_ratio, uint8_t multiplier, float sample_time_s);
void EncoderSpeed_TimerCallback(void);

float EncoderSpeed_GetRPM(void);
EncoderDirection_t EncoderSpeed_GetDirection(void);
float EncoderSpeed_GetPosition(void);

void EncoderSpeed_TimerCallback_right(void);

float EncoderSpeed_GetRPM_right(void);
EncoderDirection_t EncoderSpeed_GetDirection_right(void);
float EncoderSpeed_GetPosition_right(void);
#ifdef __cplusplus
}
#endif

#endif // ENCODER_H

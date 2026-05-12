/**
 * @file drv_encoder.c
 * @brief 编码器测速为后续PID进行服务
 *
 * This file contains implementation of initialization and processing
 * functions for the drv_encoder module.
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

#include "drv_encoder.h"

/*============================ INCLUDES ======================================*/
#include <math.h>
#include "tim.h"
#include <stdlib.h>
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
static EncoderSpeedHandle_t encoderSpeed;
/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
/**
 * @brief 编码器测速初始化函数
 * 
 * @param pulses_per_rev     编码器每圈的脉冲数（PPR），例如500
 * @param gear_ratio         电机减速比，例如34表示1:34
 * @param multiplier         编码器计数倍频数，取值1/2/4（例如使用TI1+TI2模式时为4）
 * @param sample_time_s      速度采样周期（单位：秒），与TIM6中断周期一致
 */
void EncoderSpeed_Init(uint16_t pulses_per_rev, uint8_t gear_ratio, uint8_t multiplier, float sample_time_s)
{
    encoderSpeed.last_cnt = __HAL_TIM_GET_COUNTER(&htim3);
    encoderSpeed.motor_rpm = 0.0f;
    encoderSpeed.direction = ENCODER_DIR_FORWARD;
    encoderSpeed.motor_position_revolutions = 0.0f;
    encoderSpeed.sample_time_s = sample_time_s;

    encoderSpeed.pulses_per_motor_revolution = pulses_per_rev * multiplier * gear_ratio;

    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&htim6);
}
void EncoderSpeed_TimerCallback(void)
{
    uint16_t curr_cnt = __HAL_TIM_GET_COUNTER(&htim3);
    int32_t delta_cnt = (int32_t)(curr_cnt - encoderSpeed.last_cnt);

    // 处理16位计数器溢出（双向）
    if (delta_cnt > 32767)
        delta_cnt -= 65536;
    else if (delta_cnt < -32768)
        delta_cnt += 65536;

    encoderSpeed.last_cnt = curr_cnt;

    // 根据delta_cnt符号判断方向
    if (delta_cnt > 0)
        encoderSpeed.direction = ENCODER_DIR_FORWARD;
    else if (delta_cnt < 0)
        encoderSpeed.direction = ENCODER_DIR_REVERSE;
    else
        encoderSpeed.direction = ENCODER_DIR_STOP;

    // 位置累加，delta_cnt带符号
    encoderSpeed.motor_position_revolutions += ((float)delta_cnt / encoderSpeed.pulses_per_motor_revolution);

    // 转速计算（用绝对值，单位RPM）
    float rev_per_sec = (float)(abs(delta_cnt)) / (encoderSpeed.sample_time_s * encoderSpeed.pulses_per_motor_revolution);
    encoderSpeed.motor_rpm = rev_per_sec * 60.0f;
}

/*
void EncoderSpeed_TimerCallback(void)
{
    uint32_t curr_cnt = __HAL_TIM_GET_COUNTER(&htim3);
    int32_t delta_cnt;

    if (curr_cnt >= encoderSpeed.last_cnt)
        delta_cnt = curr_cnt - encoderSpeed.last_cnt;
    else
        delta_cnt = (int32_t)((0xFFFF - encoderSpeed.last_cnt) + curr_cnt + 1);

    // 判断方向并处理 delta 符号
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3)) {
        encoderSpeed.direction = ENCODER_DIR_REVERSE;
        delta_cnt = -delta_cnt;
    } else {
        encoderSpeed.direction = ENCODER_DIR_FORWARD;
    }

    encoderSpeed.last_cnt = curr_cnt;

    // 累计位置更新
    encoderSpeed.motor_position_revolutions += ((float)delta_cnt / encoderSpeed.pulses_per_motor_revolution);

    // 转速计算（取绝对值）
    float rev_per_sec = (float)(abs(delta_cnt)) / (encoderSpeed.sample_time_s * encoderSpeed.pulses_per_motor_revolution);
    encoderSpeed.motor_rpm = rev_per_sec * 60.0f;
}
*/
float EncoderSpeed_GetRPM(void)
{
    return encoderSpeed.motor_rpm;
}

EncoderDirection_t EncoderSpeed_GetDirection(void)
{
    return encoderSpeed.direction;
}

float EncoderSpeed_GetPosition(void)
{
    return encoderSpeed.motor_position_revolutions;
}

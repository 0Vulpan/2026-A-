/**
 * @file control_pid.c
 * @brief a
 *
 * This file contains implementation of initialization and processing
 * functions for the control_pid module.
 *
 * @author chy
 * @version 1.0.0
 * @date 2026-03-25
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-03-25	chy	Initial version
 */

#include "control_pid.h"

/*============================ INCLUDES ======================================*/

#include <math.h>
#include <main.h>
#include "encoder.h"
#include "log.h"
#include "service_car.h"

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/
static PID_Handle_t motor_pid;
static Ramp_t speed_ramp;
static LowPassFilter_t speed_filter;

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
/**
 * @brief 初始化 PID 控制器参数
 * 
 * @param pid           PID 控制器句柄
 * @param Kp            比例系数
 * @param Ki            积分系数
 * @param Kd            微分系数
 * @param output_min    输出下限（例如 -1000）
 * @param output_max    输出上限（例如 1000）
 */
static void PID_Init(PID_Handle_t *pid, float Kp, float Ki, float Kd, float output_min, float output_max)
{
    pid->Kp_left = Kp;
    pid->Ki_left = Ki;
    pid->Kd_left = Kd;
    pid->setpoint_left = 0.0f;
    pid->integral_left = 0.0f;
    pid->last_error_left = 0.0f;
    pid->output_min_left = output_min;
    pid->output_max_left = output_max;
	
	pid->Kp_right = Kp;
    pid->Ki_right = Ki;
    pid->Kd_right = Kd;
    pid->setpoint_right = 0.0f;
    pid->integral_right = 0.0f;
    pid->last_error_right = 0.0f;
    pid->output_min_right = output_min;
    pid->output_max_right = output_max;
}
/**
 * @brief 计算 PID 输出值
 * 
 * @param pid       PID 控制器句柄
 * @param feedback  实际测量值（反馈）
 * @param dt        采样周期（单位：秒）
 * @return float    PID 输出值（已限幅）
 */
static float PID_Compute(PID_Handle_t *pid, float feedback, float dt)
{
    float error = pid->setpoint_left - feedback;

    pid->integral_left += error * dt;
    if (pid->integral_left > pid->output_max_left) pid->integral_left = pid->output_max_left;
    else if (pid->integral_left < pid->output_min_left) pid->integral_left = pid->output_min_left;

    float derivative = (error - pid->last_error_left) / dt;

    float output = pid->Kp_left * error + pid->Ki_left * pid->integral_left + pid->Kd_left * derivative;

    if (output > pid->output_max_left) output = pid->output_max_left;
    else if (output < pid->output_min_left) output = pid->output_min_left;

    pid->last_error_left = error;

    return output;
}
/**
 * @brief 初始化软启动斜坡参数
 * 
 * @param ramp             软启动结构体指针
 * @param target           最终目标值（如目标转速 RPM）
 * @param ramp_time_s      软启动时间（单位：秒）
 * @param sample_time_s    每次更新周期（单位：秒）
 */
static void Ramp_Init(Ramp_t *ramp, float target, float ramp_time_s, float sample_time_s)
{
    ramp->target = target;
    ramp->current = 0.0f;
    ramp->step = target / (ramp_time_s / sample_time_s);
}
/**
 * @brief 获取软启动当前目标值（每次递增靠近目标）
 * 
 * @param ramp       软启动结构体指针
 * @return float     当前步进后的目标值
 */
static float Ramp_Update(Ramp_t *ramp)
{
    if (ramp->current < ramp->target) {
        ramp->current += ramp->step;
        if (ramp->current > ramp->target)
            ramp->current = ramp->target;
    }
    return ramp->current;
}
/**
 * @brief 初始化一阶低通滤波器
 * 
 * @param lpf     滤波器结构体指针
 * @param alpha   滤波因子（0.0~1.0），越小越平滑
 */
static void LPF_Init(LowPassFilter_t *lpf, float alpha)
{
    lpf->filtered_value = 0.0f;
    lpf->alpha = alpha;
}
/**
 * @brief 对输入值执行一阶低通滤波
 * 
 * @param lpf       滤波器结构体指针
 * @param input     当前采样值
 * @return float    滤波后的输出
 */
static float LPF_Update(LowPassFilter_t *lpf, float input)
{
    lpf->filtered_value += lpf->alpha * (input - lpf->filtered_value);
    return lpf->filtered_value;
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/
/**
 * @brief PID速度控制模块初始化（含PID参数、软启动、滤波）
 */
void PID_Control_Init(void)
{
    PID_Init(&motor_pid, 2.2f, 0.5f, 0.07f, -1000.0f, 1000.0f);  // PID参数
    Ramp_Init(&speed_ramp, 200.0f, 1.0f, 0.05f);                  // 目标280RPM，3秒爬坡
    LPF_Init(&speed_filter, 0.2f);                                // 滤波因子
}

/**
 * @brief 每50ms定时调用，用于PID计算+输出PWM
 */
void PID_Control_Update(void)
{
    EncoderSpeed_TimerCallback();
	EncoderSpeed_TimerCallback_right();  // 更新编码器

    motor_pid.setpoint_left = Ramp_Update(&speed_ramp);  // 平滑目标
	motor_pid.setpoint_right = Ramp_Update(&speed_ramp);
	
    float raw_rpm_left = EncoderSpeed_GetRPM();
	float raw_rpm_right = EncoderSpeed_GetRPM_right();
	
    float filtered_rpm_left = LPF_Update(&speed_filter, raw_rpm_left);
	float filtered_rpm_right = LPF_Update(&speed_filter, raw_rpm_right);
	
    float output_left = PID_Compute(&motor_pid, filtered_rpm_left, 0.05f);  // 50ms周期
	 float output_right = PID_Compute(&motor_pid, filtered_rpm_right, 0.05f);  // 50ms周期
	
    Motor_Left_Set((int16_t)output_left);  // 输出PWM
	Motor_Right_Set((int16_t)output_right);  // 输出PWM
}




/*
这个是自整定PID参数代码需要调试我没有调试想用自己调试
void PID_AutoTuner_Init(PID_AutoTuner_t *tuner, PID_Handle_t *pid, float step_size)
{
    tuner->Ku = 0.0f;
    tuner->Tu = 0.0f;
    tuner->step_size = step_size;
    tuner->last_rpm = 0.0f;
    tuner->stable_time = 0.0f;
    tuner->step_started = 0;
    tuner->tick_start = HAL_GetTick();
    tuner->pid = pid;

    PID_Init(pid, 1.0f, 0.0f, 0.0f, -1000, 1000);  // Ku=1.0开始整定（范围可外部调）
    pid->setpoint = 0;
}

void PID_AutoTuner_Update(PID_AutoTuner_t *tuner, float measured_rpm)
{
    uint32_t now = HAL_GetTick();

    if (!tuner->step_started)
    {
        tuner->pid->setpoint = tuner->step_size;
        tuner->tick_start = now;
        tuner->step_started = 1;
        return;
    }

    float delta_rpm = fabsf(measured_rpm - tuner->last_rpm);
    tuner->last_rpm = measured_rpm;

    if (delta_rpm < 2.0f)
        tuner->stable_time += 0.05f;
    else
        tuner->stable_time = 0.0f;

    if (tuner->stable_time > 1.0f)
    {
        tuner->Tu = (now - tuner->tick_start) / 1000.0f;
        tuner->Ku = tuner->pid->Kp;

        // Ziegler-Nichols PI 参数估算
        tuner->pid->Kp = 0.45f * tuner->Ku;
        tuner->pid->Ki = 1.2f * tuner->Ku / tuner->Tu;
        tuner->pid->Kd = 0.0f;

        tuner->step_started = 0;
        tuner->pid->setpoint = 0;

        LOG_I("PID Auto-Tune Complete: Kp=%.3f, Ki=%.3f, Tu=%.3fs\r\n",
               tuner->pid->Kp, tuner->pid->Ki, tuner->Tu);
    }
}
*/


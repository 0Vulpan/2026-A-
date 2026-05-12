/**
 * @file service_car.c
 * @brief a
 *
 * This file contains implementation of initialization and processing
 * functions for the service_car module.
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

#include "service_car.h"

/*============================ INCLUDES ======================================*/
#include "main.h"
#include "gw_xunji.h"
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
static void drv8876_reset(void)
{
	HAL_GPIO_WritePin(SLEEP1_GPIO_Port,SLEEP1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SLEEP2_GPIO_Port,SLEEP2_Pin,GPIO_PIN_RESET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(SLEEP1_GPIO_Port,SLEEP1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SLEEP2_GPIO_Port,SLEEP2_Pin,GPIO_PIN_SET);
	LOG_I("drv8876 reset success");
}

void car_init(void)
{
	drv8876_reset();
	HAL_Delay(1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	LOG_I("drv8876 init success");
}

void Motor_Left_Set(int16_t speed)
{
    uint16_t pwm = (speed < 0) ? -speed : speed;
    // 限制最大占空比
    if (pwm > Pwm_input_max) pwm = Pwm_input_max;
    // 最小门槛保护
    if (pwm > 0 && pwm < Pwm_min) pwm = Pwm_min;
    if (speed == 0)
    {
        // 停止或刹车
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

    }
    else if (speed > 0)
    {
        // 正转
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

    }
    else
    {
        // 反转
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm);
    }
}

void Motor_Right_Set(int16_t speed)
{
    uint16_t pwm = (speed < 0) ? -speed : speed;

    // 限制最大占空比
    if (pwm > Pwm_input_max) pwm = Pwm_input_max;

    // 最小门槛保护
    if (pwm > 0 && pwm < Pwm_min) pwm = Pwm_min;

    if (speed == 0)
    {
        // 停止或刹车
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    }
    else if (speed > 0)
    {
        // 正转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    }
    else
    {
        // 反转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm);
    }
}

void Motor_set_pid(int16_t speed)
{
    uint16_t pwm = (speed < 0) ? -speed : speed;

    // 限制最大占空比
    if (pwm > Pwm_input_max) pwm = Pwm_input_max;

    // 最小门槛保护
    if (pwm > 0 && pwm < Pwm_min) pwm = Pwm_min;
	
	float PID_value = xunji_pid();
	
    float pwm_left = pwm -PID_value;
	float pwm_right = pwm + PID_value;
	LOG_I("PID_value:%.2f pwm_left;%.2f pwm_right:%.2f",PID_value,pwm_left,pwm_right);
    if (speed == 0)
    {
        // 停止或刹车
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    }
	
    else if (speed > 0)
    {
        // 正转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_left);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_right);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    }
    else
    {
        // 反转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_left);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm_right);
    }
}

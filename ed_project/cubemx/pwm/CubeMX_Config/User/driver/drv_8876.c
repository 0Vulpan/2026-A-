/**
 * @file drv_8876.c
 * @brief DRV8876的底层驱动
 *
 * This file contains implementation of initialization and processing
 * functions for the drv_8876 module.
 *
 * @author Xue
 * @version 1.0.0
 * @date 2025-07-02
 *
 * @Copyright (c) 2025, Xue Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2025-07-02	Xue	Initial version
 * 接线方式是PC0-IN1 PC1-IN2
 */

#include "drv_8876.h"

/*============================ INCLUDES ======================================*/
#include "adc.h"
#include "tim.h"
#include "stm32g4xx.h"                  // Device header
#include "gpio.h"
#include "log.h"
#include "delay.h"
/*============================ MACROS ========================================*/
#define AIPROPI_GAIN     0.001f      // 1000 μA/A
#define RIPROPI_RESISTOR 1500.0f     // 1.5kΩ
#define VREF_ADC         3.3f        // ADC 参考电压
#define PWM_DUTY_MIN 540		// 这个最小占空比是根据测量得到的每个电机可能不一样
														//（测量方法就是不断给一个值 520 500等等直到电机稳定启动）
#define PWM_DUTY_MAX 1000  	//  ARR = 999（1000步）
#define ADC_BUFFER_SIZE 64  // 可调整为你需要的窗口大小
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/
static uint16_t adc_dma_buffer[ADC_BUFFER_SIZE];
static volatile uint8_t fault_flag = 0;
static DelayTimer fault_check_timer;
/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
static void drv8876_reset(void)
{
	HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_RESET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_SET);
	LOG_I("drv8876 reset success");
}
//这个电流检测不是很准确会有明显的数据杂波只用于简单的过流保护和斩波调节
//测量后发现：100ma不能实时100ma会有浮动80ma左右，这个是电机驱动本身问题
//使用时，如果不需要电流环可以只作为一个显示电流保护业务
static float Get_Motor_Current(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++)
        sum += adc_dma_buffer[i];

    float adc_avg = sum / (float)ADC_BUFFER_SIZE;
    float vipropi = (adc_avg / 4095.0f) * VREF_ADC;
    float current = vipropi / (AIPROPI_GAIN * RIPROPI_RESISTOR); // 单位 A

    return current;
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/
void fault_irqhandler_callback(void)
{
    fault_flag = 1;
}
void fault_process(void)
{
		static uint8_t timer_started = 0;

    if (fault_flag && !timer_started)
    {
        fault_check_timer.restart(&fault_check_timer);
        timer_started = 1;
    }

    if (timer_started && fault_check_timer.is_expired(&fault_check_timer))
    {
        timer_started = 0;
        fault_flag = 0;

        if (HAL_GPIO_ReadPin(FAULT_GPIO_Port, FAULT_Pin) == GPIO_PIN_RESET)
        {
            // 确认FAULT，执行保护
        }
        else
        {
            //LOG_W("FAULT noise or transient, ignored");
        }
    }
}
void drv8876_init(void)
{
	drv8876_reset();
	DelayTimer_ctor_ms(&fault_check_timer, 1);  // 1ms 防抖确认延时
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	//开启电流采样
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma_buffer, ADC_BUFFER_SIZE);
	LOG_I("drv8876 init success");
}
void Set_PWM_Duty(int16_t duty)
{
    uint16_t pwm = (duty < 0) ? -duty : duty;

    // 限制最大占空比
    if (pwm > PWM_DUTY_MAX) pwm = PWM_DUTY_MAX;

    // 最小门槛保护
    if (pwm > 0 && pwm < PWM_DUTY_MIN) pwm = PWM_DUTY_MIN;

    if (duty == 0)
    {
        // 停止或刹车
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    }
    else if (duty > 0)
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

/**
 * @file delay.c
 * @brief 使用硬件定时器实现非阻塞函数
 *
 * This file contains implementation of initialization and processing
 * functions for the delay module.
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
 */

#include "delay.h"

/*============================ INCLUDES ======================================*/
#include "tim.h"
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
/**
 * @brief 获取当前时间（us），基于 TIM7 硬件计数器
 */
static inline uint32_t micros(void)
{
    return __HAL_TIM_GET_COUNTER(&htim7);
}

// 成员方法实现：
static void DelayTimer_start(DelayTimer *self, uint32_t timeout_us)
{
    self->start_time = micros();
    self->timeout_us = timeout_us;
}

static void DelayTimer_restart(DelayTimer *self)
{
    self->start_time = micros();
}

static uint8_t DelayTimer_is_expired(DelayTimer *self)
{
    return (uint16_t)(micros() - self->start_time) >= self->timeout_us;
}

static uint32_t DelayTimer_elapsed(DelayTimer *self)
{
    return micros() - self->start_time;
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/
void DelayTimer_ctor_us(DelayTimer *self, uint32_t timeout_us)
{
    self->start = DelayTimer_start;
    self->restart = DelayTimer_restart;
    self->is_expired = DelayTimer_is_expired;
    self->elapsed = DelayTimer_elapsed;

    self->start(self, timeout_us); // 初始化时启动
}
void DelayTimer_ctor_ms(DelayTimer *self, uint32_t timeout_ms)
{
    DelayTimer_ctor_us(self, timeout_ms * 1000);  // 复用 us 构造函数
}
void DelayTimer_GlobalInit(void)
{
    HAL_TIM_Base_Start(&htim7); // 启动硬件定时器
}
/*
#include "delay.h"

DelayTimer led_timer;
DelayTimer key_timer;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM17_Init();

    DelayTimer_GlobalInit();

    DelayTimer_ctor(&led_timer, 500000); // LED 500ms
    DelayTimer_ctor(&key_timer, 10000);  // 按键消抖 10ms

    while (1)
    {
        if (led_timer.is_expired(&led_timer)) {
            led_timer.restart(&led_timer);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }

        if (key_timer.is_expired(&key_timer)) {
            key_timer.restart(&key_timer);
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) {
                // 检测到按键按下
            }
        }
    }
}
*/

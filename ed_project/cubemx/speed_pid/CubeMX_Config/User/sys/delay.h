/**
 * @file delay.h
 * @brief 使用硬件定时器实现非阻塞函数
 *
 * This file contains function declarations and necessary includes
 * for the delay module.
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

#ifndef DELAY_H
#define DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/
#include "stm32g4xx.h"                  // Device header
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
/**
* @brief DelayTimer结构体，模拟C++类行为
*        每个任务/模块创建一个 DelayTimer 实例，用于独立延时控制
*/
typedef struct DelayTimer DelayTimer;
struct DelayTimer {
    uint32_t start_time;      // 起始时间戳（us）
    uint32_t timeout_us;      // 设定的延时时间（us）

    // 成员函数指针（模拟类方法）
    void     (*start)(DelayTimer *self, uint32_t timeout_us);
    void     (*restart)(DelayTimer *self);
    uint8_t  (*is_expired)(DelayTimer *self);
    uint32_t (*elapsed)(DelayTimer *self);
};
/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
// 微秒构造函数
void DelayTimer_ctor_us(DelayTimer *self, uint32_t timeout_us);
// 毫秒构造函数
void DelayTimer_ctor_ms(DelayTimer *self, uint32_t timeout_ms);
// 初始化 TIM17 硬件计数器（只调用一次）
void DelayTimer_GlobalInit(void);
#ifdef __cplusplus
}
#endif

#endif // DELAY_H

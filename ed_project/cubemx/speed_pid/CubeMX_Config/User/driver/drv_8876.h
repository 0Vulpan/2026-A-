/**
 * @file drv_8876.h
 * @brief DRV8876的底层驱动
 *
 * This file contains function declarations and necessary includes
 * for the drv_8876 module.
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

#ifndef DRV_8876_H
#define DRV_8876_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/
#define AIPROPI_GAIN     0.001f      // 1000 μA/A
#define RIPROPI_RESISTOR 1500.0f     // 1.5kΩ
#define VREF_ADC         3.3f        // ADC 参考电压
#define PWM_DUTY_MIN 540		// 这个最小占空比是根据测量得到的每个电机可能不一样
														//（测量方法就是不断给一个值 520 500等等直到电机稳定启动）
#define PWM_DUTY_MAX 1000  	//  ARR = 999（1000步）
#define PWM_INPUT_MAX 1000
#define PWM_INPUT_DEADZONE 30   // 小于这个值输出0，避免微抖动
#define ADC_BUFFER_SIZE 64  // 可调整为你需要的窗口大小
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void drv8876_init(void);
void Set_PWM_Duty(int16_t duty);
void fault_irqhandler_callback(void);	
float Get_Motor_Current(void);
	void fault_process(void);
#ifdef __cplusplus
}
#endif

#endif // DRV_8876_H

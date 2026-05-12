/**
 * @file drv_8876.h
 * @brief DRV8876µÄµ×²ãÇý¶¯
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

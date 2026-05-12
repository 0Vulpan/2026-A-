/**
 * @file My_PWM.h
 * @brief hg
 *
 * This file contains function declarations and necessary includes
 * for the My_PWM module.
 *
 * @author fv
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	fv	Initial version
 */

#ifndef MY_PWM_H
#define MY_PWM_H

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
#define PWM_MAX       19999
void PWM_Init(void);
void duoji_zhuan_one(uint16_t angle);
void duoji_zhuan_two(uint16_t angle);
void dianji_zhuan_one(uint8_t speed);
void dianji_zhuan_two(uint8_t speed);
void Motor_Stop(void);
void Motor_Error();
void Motor_Right();
#ifdef __cplusplus
}
#endif

#endif // MY_PWM_H

/**1
 * @file service_car.h
 * @brief a
 *
 * This file contains function declarations and necessary includes
 * for the service_car module.
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

#ifndef SERVICE_CAR_H
#define SERVICE_CAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/
#define Pwm_min 200
#define Pwm_max 1000
#define Pwm_input_max 1000
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void car_init(void);

void Motor_Left_Set(int16_t speed);
void Motor_Right_Set(int16_t speed);
void Motor_set_pid(int16_t speed);
#ifdef __cplusplus
}
#endif

#endif // SERVICE_CAR_H

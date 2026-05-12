/**1
 * @file control_pid.h
 * @brief a
 *
 * This file contains function declarations and necessary includes
 * for the control_pid module.
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

#ifndef CONTROL_PID_H
#define CONTROL_PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
typedef struct {
    float Kp_left;
    float Ki_left;
    float Kd_left;
    float setpoint_left;
    float integral_left;
    float last_error_left;
    float output_min_left;
    float output_max_left;
	
	float Kp_right;
    float Ki_right;
    float Kd_right;
    float setpoint_right;
    float integral_right;
    float last_error_right;
    float output_min_right;
    float output_max_right;
	
	
} PID_Handle_t;

typedef struct {
    float target;
    float current;
    float step;
} Ramp_t;

typedef struct {
    float filtered_value;
    float alpha;
} LowPassFilter_t;

typedef struct {
    float Ku;
    float Tu;
    float step_size;
    float last_rpm;
    float stable_time;
    uint8_t step_started;
    uint32_t tick_start;
    PID_Handle_t *pid;
} PID_AutoTuner_t;
/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/

void PID_Control_Update(void);
void PID_AutoTuner_Init(PID_AutoTuner_t *tuner, PID_Handle_t *pid, float step_size);
void PID_AutoTuner_Update(PID_AutoTuner_t *tuner, float measured_rpm);

// ≥ı ºªØ
void PID_Control_Init(void);

#ifdef __cplusplus
}
#endif

#endif // CONTROL_PID_H

/**
 * @file pid.h
 * @brief ËÙ¶È»·PID
 *
 * This file contains function declarations and necessary includes
 * for the pid module.
 *
 * @author Xue
 * @version 1.0.0
 * @date 2025-07-05
 *
 * @Copyright (c) 2025, Xue Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2025-07-05	Xue	Initial version
 */

#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/
#include "stm32g4xx.h"                  // Device header

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float integral;
    float last_error;
    float output_min;
    float output_max;
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
void PID_Control_Init(void);
void PID_Control_Update(void);
void PID_AutoTuner_Init(PID_AutoTuner_t *tuner, PID_Handle_t *pid, float step_size);
void PID_AutoTuner_Update(PID_AutoTuner_t *tuner, float measured_rpm);
#ifdef __cplusplus
}
#endif

#endif // PID_H

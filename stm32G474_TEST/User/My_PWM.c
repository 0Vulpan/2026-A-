/**
 * @file My_PWM.c
 * @brief hg
 *
 * This file contains implementation of initialization and processing
 * functions for the My_PWM module.
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

#include "My_PWM.h"
#include "tim.h"

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

void duoji_zhuan_one(uint16_t angle)//���ת��
{
  uint32_t s;

    if(angle > 180) angle = 180;
    if(angle < 0)   angle = 0;

    // 0�� -> 500
    // 180�� -> 2500
    s = 500 + (angle* 2000) / 180;

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, s);
}
void duoji_zhuan_two(uint16_t angle)//���ת��
{
  uint32_t s;

    if(angle > 180) angle = 180;
    if(angle < 0)   angle = 0;

    // 0�� -> 500
    // 180�� -> 2500
    s = 500 + (angle * 2000)/ 180;

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, s);
}
void Motor_Right()//�����ת
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET);
}
void Motor_Error()//�����ת
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET);
}
void Motor_Stop(void)//����ֹͣ
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET);
    
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}
void dianji_zhuan_one(uint8_t speed)//���ת��
{
   uint32_t s;
	if(speed>100) speed=100;
	
	s=(speed*PWM_MAX)/100;

  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,s);
	LOG_I("Motor PWM speed=%d, compare=%lu", speed, s);
}
void dianji_zhuan_two(uint8_t speed)//���ת��
{
   uint32_t s;
	if(speed>100) speed=100;
	
	s=(speed*PWM_MAX)/100;

  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,s);
	//LOG_I("��� PWM �ٶ� =%d %%\r\n",speed);
}

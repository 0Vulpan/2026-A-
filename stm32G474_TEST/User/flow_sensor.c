/**
 * @file flow_sensor.c
 * @brief 1
 *
 * This file contains implementation of initialization and processing
 * functions for the flow_sensor module.
 *
 * @author 1
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	1	Initial version
 */

#include "flow_sensor.h"

/*============================ INCLUDES ======================================*/
#include "log.h"
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/


#include "tim.h"
golbal_flow_t golbal_flow;
float flowK[3] = {5.0f, 5.5f, 80.0f};                  // ����ϵ�� K
float pulseCntPerLiter[3] = {300.0f, 330.0f, 4800.0f}; // ÿ��ˮ������
FlowModel flowModel = MODE_6MM_PIPE;
uint32_t pluse1L=0;
void flow_sensor_Init()
{
HAL_TIM_Base_Start_IT(&htim7);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim7))
    {
        golbal_flow.receive_flag = 1;
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t last_pulse_time = 0;
    if(GPIO_Pin == YFS401_Pin)
	{
		if((HAL_GetTick() - last_pulse_time) > 5)
		{
        golbal_flow.pluse_1s++;
		last_pulse_time = HAL_GetTick();
		}           //�½��زɼ�������
    }
}
void Flow_Read(void)
{
    // ���� Flow_Model ѡ��ͬ����������
    float flowKValue = flowK[flowModel];
    float pulseCntValue = pulseCntPerLiter[flowModel];
    
	if(golbal_flow.pluse_1s > 0)
	{
		golbal_flow.acculat += ((float)golbal_flow.pluse_1s * 1000.0f / pulseCntValue);   //��λmL
		pluse1L+=golbal_flow.pluse_1s;
		golbal_flow.instant = (float)golbal_flow.pluse_1s/ flowKValue;  //��λ��L/min��

        if(golbal_flow.acculat >= 1000000)        //����ۼ�����1000L
		{
			golbal_flow.acculat = 0;
		}
	}
	else
	{
		golbal_flow.instant  = 0;
	}
	
	LOG_I("Instant Flow = %.2f L/min, Accumulated = %.2f mL, Pulse = %d\r\n", golbal_flow.instant, golbal_flow.acculat, golbal_flow.pluse_1s);
   
	golbal_flow.receive_flag = 0;   //������ɱ�־λ����
    golbal_flow.pluse_1s = 0;       //����������
}

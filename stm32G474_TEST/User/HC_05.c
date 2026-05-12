/**
 * @file HC_05.c
 * @brief 11
 *
 * This file contains implementation of initialization and processing
 * functions for the HC_05 module.
 *
 * @author 05
 * @version 1.0.0
 * @date 2026-04-23
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-23	05	Initial version
 */

#include "HC_05.h"

/*============================ INCLUDES ======================================*/
extern UART_HandleTypeDef huart2;
uint8_t rx_data;

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/


/*============================ EXTERNAL IMPLEMENTATION =======================*/
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // 判断是否是串口1触发的中断
    if(huart->Instance == USART2)
    {
			
				if(rx_data == char)
        {
           printf("1");
           
        }
        else if(rx_data == '0')
        {
             printf("0");
        }
        HAL_UART_Receive_IT(&huart2, &rx_data, );
    }
}

void blue_init(void)
{

	if(HAL_UART_Receive_IT(&huart2, &rx_data, 1)==HAL_OK){
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)"blue init success", sizeof("blue init success"));
	}else{
		
			HAL_UART_Transmit_DMA(&huart2, (uint8_t *)"blue init fail", sizeof("blue init fail"));
	}
}

#endif


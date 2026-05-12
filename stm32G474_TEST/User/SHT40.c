/**
 * @file SHT40.c
 * @brief 11
 *
 * This file contains implementation of initialization and processing
 * functions for the SHT40 module.
 *
 * @author 40
 * @version 1.0.0
 * @date 2026-04-24
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-24	40	Initial version
 */

#include "SHT40.h"
/*============================ INCLUDES ======================================*/
extern UART_HandleTypeDef huart3;
/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
SHT40_DataTypeDef sht40_data = {0};
uint8_t sht40_rx_buf[SHT40_RX_BUF_SIZE] = {0};
uint8_t sht40_rx_len = 0;
/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
void SHT40_SendCmd(const char *cmd)
{
    char send_buf[16] = {0};
    sprintf(send_buf, "%s\r\n", cmd); 
    HAL_UART_Transmit(&huart3, (uint8_t *)send_buf, strlen(send_buf), 100);
}

void SHT40_Init(SHT40_ModeTypeDef mode)
{   
	  HAL_UART_Receive_IT(&huart3,sht40_rx_buf, 1);
    HAL_Delay(100); 
        if(mode == SHT40_MODE_AUTO)
    {
        SHT40_SendCmd("Auto");
    }
    else
    {
        SHT40_SendCmd("Hand");
    }
    HAL_Delay(100);
}

/*============================ EXTERNAL IMPLEMENTATION =======================*/

uint8_t SHT40_ParseData(uint8_t *buf, uint16_t len)
{
    sht40_data.data_valid = 0;
    
    if(strstr((char *)buf, "R:") == NULL || strstr((char *)buf, "RH") == NULL || strstr((char *)buf, "C") == NULL)
    {
        return 0; 
    }
    
    float hum = 0.0f;
    char *rh_ptr = strstr((char *)buf, "R:");
    if(rh_ptr != NULL)
    {
        sscanf(rh_ptr, "R:%fRH", &hum); 
        sht40_data.humidity = hum;
    }
        float temp = 0.0f;
    char *temp_ptr = strstr((char *)buf, "RH ");
    if(temp_ptr != NULL)
    {
        temp_ptr += 3; 
        sscanf(temp_ptr, "%fC", &temp); 
        sht40_data.temperature = temp;
    }
        if(hum >= 0.0f && hum <= 100.0f && temp >= -40.0f && temp <= 125.0f)
    {
        sht40_data.data_valid = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

void SHT40_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huart3.Instance)
    {
        if(sht40_rx_buf[sht40_rx_len] == '\n')
        {
            if(SHT40_ParseData(sht40_rx_buf, sht40_rx_len + 1))
            {
                //data_update_flag = 1;
            }
                 memset(sht40_rx_buf, 0, SHT40_RX_BUF_SIZE);
            sht40_rx_len = 0;
        }
        else
        {
            sht40_rx_len++;
            if(sht40_rx_len >= SHT40_RX_BUF_SIZE)
            {
                memset(sht40_rx_buf, 0, SHT40_RX_BUF_SIZE);
                sht40_rx_len = 0;
            }
        }
                HAL_UART_Receive_IT(&huart3, &sht40_rx_buf[sht40_rx_len], 1);
    }
}




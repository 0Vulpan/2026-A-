/**
 * @file MY_ADC.c
 * @brief QQW
 *
 * This file contains implementation of initialization and processing
 * functions for the MY_ADC module.
 *
 * @author QW
 * @version 1.0.0
 * @date 2026-04-25
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-25	QW	Initial version
 */

#include "main.h"
#include "adc.h"
#include "MY_ADC.h"
#include "math.h"
#include "log.h"
/*============================ INCLUDES ======================================*/
#define ADC_CH_NUM        3    
#define ADC_FILTER_COUNT  20  
#define ADC_REF_VOLTAGE   3.3f
#define ADC_RESOLUTION    4095.0f
#define INA240_GAIN       100.0f   // 实际增益
#define RSENSE            0.1f   // 实际采样电阻
#define VREF            1.65f
#define VREF_MV        3300    // 参考电压 3.3V
#define DRY_ADC        4000    // 无水时 ADC 值
#define FULL_ADC       1000    // 满水位 70mm 时 ADC 值
#define MAX_HEIGHT     70      // 最大测量高度 70mm
/*============================ MACROS ========================================*/
uint32_t adc_sum_ch0 = 0;
uint8_t  adc_cnt_ch0 = 0;
float    voltage_ch0 = 0.0f;
uint16_t light_ch0   = 0;

uint16_t adc_buf[ADC_CH_NUM];

uint32_t adc_buf_dr[ADC_CH_NUM];
uint32_t adc_buffer[ADC_CH_NUM];

uint32_t adc_sum_ch1 = 0;
uint8_t  adc_cnt_ch1 = 0;
float    voltage_ch1 = 0.0f;
float    current = 0.0f;
float    current_mA = 0.0f;

uint32_t adc_sum_ch2 = 0;
uint8_t  adc_cnt_ch2 = 0;
uint16_t adc_ch2_value = 0;
uint16_t adc_ch2_voltage_mv = 0;
uint8_t  water_level_mm = 0;
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
void ADC_StartChannel(void)
{

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, ADC_CH_NUM);
}

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void ADC_Test(void)
{
	

	for(uint8_t i = 0; i<ADC_CH_NUM; i++)
	{
		LOG_I("ADC_buf = %d ,CH =  %d",adc_buf[i],i);
		LOG_I("V : %.2f",(float)adc_buf[i]/4096.0f * 3.30f);
		
	}
		for(uint8_t i = 0; i<ADC_CH_NUM; i++)
	{
		adc_buf[i] = 0;
	}
	
}
/*
	5V  
	3.3V ->
*/
void ADC_Process_CH0(void)
{
    adc_sum_ch0 += adc_buf[0];
    adc_cnt_ch0++;

    if(adc_cnt_ch0 >= ADC_FILTER_COUNT)
    {
        uint16_t avg = adc_sum_ch0 / adc_cnt_ch0;
        voltage_ch0 = (float)avg * 3.3f / 4095.0f;
        
        if(voltage_ch0 > 2.0f) voltage_ch0 = 2.0f;
        light_ch0 = (uint16_t)((voltage_ch0 / 2.0f) * 100.0f);
        
        LOG_I("V0: %.2fV  light:%d\r\n", voltage_ch0, light_ch0);
        
        adc_sum_ch0 = 0;
        adc_cnt_ch0 = 0;
    }
}

void ADC_Process_CH1(void)
{
    adc_sum_ch1 += adc_buf[1];  
   	adc_cnt_ch1++;

    if(adc_cnt_ch1 >= ADC_FILTER_COUNT)
    {
        uint16_t adc_avg = adc_sum_ch1 / adc_cnt_ch1;
        voltage_ch1 = ((float)adc_avg - VREF) * ADC_REF_VOLTAGE / ADC_RESOLUTION;
        current = voltage_ch1 / (INA240_GAIN * RSENSE);
        current_mA = current * 1000;
			LOG_I("CH1:I:%.3f A / %.2f mA\r\n", current, current_mA);
        adc_sum_ch1 = 0;
        adc_cnt_ch1 = 0;
	}
}

void ADC_Process_CH2(void)
{
    adc_sum_ch2 += adc_buf[2];  
    adc_cnt_ch2++;

    if(adc_cnt_ch2 >= ADC_FILTER_COUNT)
    {
        adc_ch2_value = adc_sum_ch2 / adc_cnt_ch2;
        adc_ch2_voltage_mv = (adc_ch2_value * VREF_MV) / 4095;
        if(adc_ch2_value >= DRY_ADC)
        {
            water_level_mm = 0;
        }
        else if(adc_ch2_value <= FULL_ADC)
        {
            water_level_mm = MAX_HEIGHT;
        }
        else
        {
            uint32_t delta = DRY_ADC - FULL_ADC;
            if(delta == 0) delta = 1; 

            uint32_t curr = DRY_ADC - adc_ch2_value;
            water_level_mm = (curr * MAX_HEIGHT) / delta;
        }
        LOG_I("CH2: ADC=%d, V=%dmV, Level=%dmm\r\n",
              adc_ch2_value, adc_ch2_voltage_mv, water_level_mm);
        adc_sum_ch2 = 0;
        adc_cnt_ch2 = 0;
    }
}

void ADC_Process_ALL(void)
{
    ADC_Process_CH0();
    ADC_Process_CH1();
    ADC_Process_CH2();
}

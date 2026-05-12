/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HC_05.h"
#include "software_I2C.h"
#include "bsp_oled.h"
#include "SHT40.h"
#include <stdio.h>
#include <string.h>
#include "My_PWM.h"
#include "flow_sensor.h"
#include "tjc_usart_hmi.h"

extern void LED_SetBrightness(uint8_t level);

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//啊啊啊啊
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define RX_BUFFER_SIZE 100
uint8_t rx_single_byte;
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0;
uint8_t rx_complete_flag = 0;

typedef struct {
    uint8_t enable;
    uint8_t target_flow_digit1;
    uint8_t target_flow_digit2;
    uint8_t target_flow_digit3;
    uint32_t target_flow;
    uint8_t motor_running;
} SprayControl_t;
SprayControl_t spray_control = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_UART5_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
		MyI2C_Init();
    LOG_I("Start initialization...");
    PWM_Init();		        // 先初始化PWM，启动TIM3通道
    LOG_I("PWM_Init done");
    Motor_Stop();           // 先停止电机，关闭方向GPIO
    dianji_zhuan_one(0);   // 电机1初始停止（PWM设为0）
    LOG_I("dianji_zhuan_one(0) done");
		dianji_zhuan_two(0);   // 电机2初始停止
    LOG_I("dianji_zhuan_two(0) done");
		OLED_Init();
		OLED_Clear();
		SHT40_Init(SHT40_MODE_MANUAL);
		flow_sensor_Init();
		initRingBuffer();
		USART_RX_STA = 0;
		memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
    HAL_UART_Receive_IT(&huart5, RxBuffer, 1);
		uint8_t t;
    uint8_t len;
    uint16_t times = 0;
	
		
int test_num = 0;
float test_float = 0;
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
    {
        // 每秒读取一次流量（由TIM7中断触发）
        if(golbal_flow.receive_flag) {
            Flow_Read();
        }
//		duoji_zhuan_one(90);
		 if (USART_RX_STA & 0x8000) {
            len = USART_RX_STA & 0x3fff; 
			 if(USART_RX_BUF[0]=='L')
			 {
         uint8_t brightness=0;
    sscanf((char*)&USART_RX_BUF[1], "%hhu", &brightness);
    LED_SetBrightness(brightness);
			 }				 
  else if (USART_RX_BUF[0] == '1') // 0x31
            {
							LOG_I("%s",USART_RX_BUF);
                duoji_zhuan_one(90);
               
            }
 else if (USART_RX_BUF[0] == '2') // 0x32
            {
                duoji_zhuan_one(0);
                
            }
  else if (USART_RX_BUF[0] == '3') // 0x33
            {
               duoji_zhuan_two(90);
                
            }
	else if (USART_RX_BUF[0] == '4') // 0x34
            {
                duoji_zhuan_two(0);

            }
			else if (USART_RX_BUF[0] == 'w')
            {
                LOG_I("w command received, len=%d, buf=%s", len, USART_RX_BUF);
                spray_control.target_flow = 0;
                spray_control.motor_running = 0;

                uint32_t flow_value = 0;
                if (len == 3) {
                    flow_value = (USART_RX_BUF[1] - '0') * 10 + (USART_RX_BUF[2] - '0');
                    LOG_I("len=3, parsed flow_value=%lu from %c%c", flow_value, USART_RX_BUF[1], USART_RX_BUF[2]);
                } else if (len == 4) {
                    flow_value = (USART_RX_BUF[1] - '0') * 100 + (USART_RX_BUF[2] - '0') * 10 + (USART_RX_BUF[3] - '0');
                    LOG_I("len=4, parsed flow_value=%lu from %c%c%c", flow_value, USART_RX_BUF[1], USART_RX_BUF[2], USART_RX_BUF[3]);
                } else if (len == 5) {
                    flow_value = (USART_RX_BUF[1] - '0') * 10 + (USART_RX_BUF[2] - '0');
                    LOG_I("len=5, parsed flow_value=%lu from %c%c", flow_value, USART_RX_BUF[1], USART_RX_BUF[2]);
                } else {
                    LOG_I("len=%d, flow_value=0 (default)", len);
                }
                LOG_I("Final flow_value=%lu", flow_value);
                spray_control.target_flow = flow_value;

                if (flow_value > 0)
                {
                    LOG_I("Flow > 0, starting motor speed=100");
                    dianji_zhuan_one(100);
                    spray_control.motor_running = 1;
                } else if (flow_value == 0) {
                    LOG_I("Flow = 0, STOPPING motor");
                    dianji_zhuan_one(0);
                    Motor_Stop();
                    spray_control.motor_running = 0;
                }

                if (flow_value == 0) {
                    LOG_I("Updating t4 to STOP");
                    uint8_t stop_msg[] = "t4.txt=\"\xcd\xa3\xd6\xb9\"";
                    tjc_send_nstring((char*)stop_msg, sizeof(stop_msg)-1);
                } else if (flow_value == 30) {
                    LOG_I("Updating t4 to LOW");
                    uint8_t low_msg[] = "t4.txt=\"\xb5\xcd\xc5\xe8\xc1\xbf\"";
                    tjc_send_nstring((char*)low_msg, sizeof(low_msg)-1);
                } else if (flow_value == 60) {
                    LOG_I("Updating t4 to MID");
                    uint8_t mid_msg[] = "t4.txt=\"\xd6\xd0\xc5\xe8\xc1\xbf\"";
                    tjc_send_nstring((char*)mid_msg, sizeof(mid_msg)-1);
                } else if (flow_value == 90) {
                    LOG_I("Updating t4 to HIGH");
                    uint8_t high_msg[] = "t4.txt=\"\xb8\xdf\xc5\xe8\xc1\xbf\"";
                    tjc_send_nstring((char*)high_msg, sizeof(high_msg)-1);
                } else {
                    LOG_I("flow_value=%lu, no t4 update", flow_value);
                }
            }
            USART_RX_STA = 0;
        } else {
                if (spray_control.motor_running && spray_control.target_flow >= 0 && (uint32_t)golbal_flow.acculat >= spray_control.target_flow)
                {
                    dianji_zhuan_one(0);
                    Motor_Stop();
                    spray_control.motor_running = 0;
                    tjc_send_string("page 3");
                    uint8_t complete_msg[] = "t3.txt=\"\xcd\xea\xb3\xc9\"";
                    tjc_send_nstring((char*)complete_msg, sizeof(complete_msg)-1);
                    LOG_I("Spray completed, flow reached: %.2f mL, target: %lu mL", golbal_flow.acculat, spray_control.target_flow);
                }
            }
		

	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f)
{
    uint8_t temp = ch;
    HAL_UART_Transmit(&huart1, &temp, 1, 10);
    return ch;
}

// 统一串口5接收：屏幕 + 舵机 都能用
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart5)
    {
        // 先存数据到舵机的缓冲区
        if(USART_RX_STA < 100)
        {
            USART_RX_BUF[USART_RX_STA++] = RxBuffer[0];
            LOG_I("UART rx: 0x%02X ('%c'), sta=%d", RxBuffer[0], RxBuffer[0], USART_RX_STA);
            // 收到换行，代表一帧结束（你发1、2、3、4加回车）
            if(RxBuffer[0] == '\n' || RxBuffer[0] == '\r')
            {
                USART_RX_STA |= 0x8000;
            }
        }

        

        // 给屏幕用
        HAL_UART_RxCpltCallback_0(huart);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
		
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

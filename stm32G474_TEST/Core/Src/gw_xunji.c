/**
 * @file gw_xunji.c
 * @brief a
 *
 * This file contains implementation of initialization and processing
 * functions for the gw_xunji module.
 *
 * @author chy
 * @version 1.0.0
 * @date 2026-04-15
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-15	chy	Initial version
 */

#include "gw_xunji.h"
#include "gw_iic.h"
#include "encoder.h"
#include "service_car.h"
#include "log.h"
#include "i2c.h"
/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/
#define VOLTAGE_MAX 4350 
#define THRESHOLD_VOLTAGE 4000
#define THRESHOLD_IIC 126
#define CHANNEL 8
#define I_Max 100
#define I_Min -100
/*============================ TYPES =========================================*/
unsigned char Digtal;
unsigned char Anolog[CHANNEL]={0};
unsigned char rx_buff[256]={0};
unsigned char Normal[CHANNEL]={0};
static PID_Xunji pid_init;
//volatile uint16_t adc_value[channel] = {0,0,0,0,0,0,0,0};
//uint16_t xunji_a[channel] = {0,0,0,0,0,0,0,0};
float P=0.0;
float I=0.0;
float D=0.0;
float previous_error=0.0;
float PID_value=0.0;
/*============================ GLOBAL VARIABLES ==============================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
unsigned char IIC_ReadByte(unsigned char Salve_Adress)
{
    unsigned char dat;
    HAL_I2C_Master_Receive(&hi2c3, (Salve_Adress << 1), &dat, 1, 1000);
    return dat;
}
unsigned char IIC_ReadBytes(unsigned char Salve_Adress,unsigned char Reg_Address,unsigned char *Result,unsigned char len)
{
	return HAL_I2C_Mem_Read(&hi2c3,(Salve_Adress << 1),Reg_Address,I2C_MEMADD_SIZE_8BIT,Result,len,1000)==HAL_OK;
}
unsigned char IIC_WriteByte(unsigned char Salve_Adress,unsigned char Reg_Address,unsigned char data)
{
	unsigned char dat[2]={Reg_Address,data};
	return HAL_I2C_Master_Transmit(&hi2c3,(Salve_Adress << 1),dat,2,1000)==HAL_OK;
}
unsigned char IIC_WriteBytes(unsigned char Salve_Adress,unsigned char Reg_Address,unsigned char *data,unsigned char len)
{
	return HAL_I2C_Mem_Write(&hi2c3,(Salve_Adress << 1),Reg_Address,I2C_MEMADD_SIZE_8BIT,data,len, 1000)==HAL_OK;
}
unsigned char Ping(void)
{
	unsigned char dat = 0;
	// 1. 先检查 I2C 通信是否成功
	if (IIC_ReadBytes(GW_GRAY_ADDR_DEF, GW_GRAY_PING, &dat, 1))
	{
		// 2. 再检查数据是否正确
		if (dat == GW_GRAY_PING_OK)
		{
			LOG_I("Ping Success! Got: 0x%02X", dat);
			return 0;
		}
		else
		{
			LOG_I("Ping Data Mismatch! Got: 0x%02X, Expected: 0x%02X", dat, GW_GRAY_PING_OK);
		}
	}
	else
	{
		LOG_I("Ping I2C Communication Failed!");
	}
	return 1;
}
unsigned char IIC_Get_Digtal(void)
{
	unsigned char dat;
	IIC_ReadBytes(GW_GRAY_ADDR_DEF,GW_GRAY_DIGITAL_MODE,&dat,1);
	return dat;
}
unsigned char IIC_Get_Anolog(unsigned char * Result,unsigned char len)
{
	if(IIC_ReadBytes(GW_GRAY_ADDR_DEF,GW_GRAY_ANALOG_BASE_,Result,len))return 1;
	else return 0;
}
unsigned char IIC_Get_Single_Anolog(unsigned char Channel)
{
	unsigned char dat;
	IIC_ReadBytes(GW_GRAY_ADDR_DEF,GW_GRAY_ANALOG(Channel),&dat,1);
	return dat;
}
unsigned char IIC_Anolog_Normalize(uint8_t Normalize_channel)
{
	return IIC_WriteBytes(GW_GRAY_ADDR_DEF,0xCF,&Normalize_channel,1);
}
unsigned short IIC_Get_Offset(void )
{
	unsigned char dat[2]={0};
	IIC_ReadBytes(GW_GRAY_ADDR_DEF,Offset,dat,2);
	return (unsigned short)dat[0]|(unsigned short)dat[1]<<8;
}
/*============================ EXTERNAL IMPLEMENTATION =======================*/

void gw_output(void)
{
		IIC_Anolog_Normalize(0xff);//所有通道归一化都打开
		HAL_Delay(10);//设置完，需要等上一会。stm8的运算速度没stm32快，等一下，让传感器把数据刷新一下。
		if(IIC_Get_Anolog(Normal,CHANNEL)){
		sprintf((char *)rx_buff,"Normalize %d-%d-%d-%d-%d-%d-%d-%d\r\n",Normal[0],Normal[1],Normal[2],Normal[3],Normal[4],Normal[5],Normal[6],Normal[7]);
		LOG_I("%s",rx_buff);
		memset(rx_buff,0,256);
		}
}

void gw_init(void)
{ 
	HAL_Delay(100); // 新增：等待传感器上电稳定
	while(Ping())
	{
		HAL_Delay(500);
		sprintf((char *)rx_buff,"Ping Faild Try Again!\r\n");
		LOG_I("%s",rx_buff);
		memset(rx_buff,0,256);
}
	LOG_I("Ping Success! Sensor Ready."); // 新增：Ping 成功提示
}

int read(void)
{
	int val=0;
	#if 0 
	for(int i =0 ; i<CHANNEL;i++){
		float voltage = adc_value[i] * VOLTAGE_MAX / 4095.0f;
        if(adc_value[i] > THRESHOLD_VOLTAGE){
            xunji_a[i] = 1; // 白色 灯亮
			val |= 1<<i;
        }
        else{
            xunji_a[i] = 0; // 黑色 灯灭
			val |= 0<<i;
        }
		LOG_I("xunji_a[%d]:%d",i,xunji_a[i]);
	}
	
	#endif
	if(IIC_Get_Anolog(Normal,CHANNEL)){
		for(int i = 0; i<CHANNEL; i++ ){
		if(Normal[i] > THRESHOLD_IIC){
			val |= 1<<i;
			LOG_I("Normal[%d]:%d",i,Normal[i]);
		}else{
			val |= 0<<i;
			LOG_I("Normal[%d]:%d",i,Normal[i]);
		}
	}
	
	
	}else{
		LOG_E("Get Anolog fail");
	}
	LOG_I("val:%d",val);
	return val;
}

void gw_xunji_init(void)
{
	gw_init();
	xunji_pid_init(&pid_init,34.5f,0.3f,55.00f);
}

void xunji_pid_init(PID_Xunji *pid,float Kp,float Ki, float Kd)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
}

float xunji_pid(void)
{
	float error=0.0;
	switch (read())
	{	
		case 127:  error = -6.0;     break; //0111 1111 右->左
		case 15:   error = -10.0;     break; //0000 1111
		case 7:   error = -10.0;     break; //0000 0111
		case 143:  error = -5.0;     break; //1000 1111
		case 63:   error = -8.0;     break; //0011 1111
		case 31:   error = -7.0;     break; //0001 1111
		case 159:  error = -2.0;     break; //1001 1111
		case 191:  error = -1.5;     break; //1011 1111
		case 207:  error = -1.0;     break; //1100 1111	
		case 223:  error = -1.0;  	 break; //1101 1111
		
		
		case 240:  error = 10.0;      break; //1111 0000
		case 224:  error = 10.0;      break; //1110 0000
		case 248:  error = 7.0;      break; //1111 1000
		case 252:  error = 8.0;      break; //1111 1100
		case 254:  error = 6.0;      break; //1111 1110
		case 241:  error = 5.0;      break; //1111 0001
		
		
		case 249:  error = 2.0;      break; //1111 1001
		case 253:  error = 1.5;      break; //1111 1101
		case 243:  error = 1.0;      break; //1111 0011
		case 251:  error = 1.0;   	break; //1111 1011
		
		case 239:  error = -0.5;      break; //1110 1111
		case 247:  error = 0.5;      break; //1111 0111
		case 231:  error = 0.0;      break; //1110 0111	
		
//		case 0:    error = 0;      	 break; //0000 0000
//		case 255:  error = 0;     	 break; //1111 1111
//		default:   error = 0;      	 break;
	}
	P = error;                       //P项
	I += pid_init.Ki*error;			 //I项
	D = error - previous_error;       //D项（当前误差减上次误差）
	if(I>I_Max)I=I_Max;
	if(I<I_Min)I=I_Min;
	PID_value = (pid_init.Kp * P)+ I+(pid_init.Kd * D); //核心公式
	previous_error = error; 
	LOG_I("P:%.2f I:%.2f D:%.2f PID_VALUE:%.2f",P,I,D,PID_value);
	return PID_value;
}

void gw_pid_update(void)
{
	EncoderSpeed_TimerCallback();
	EncoderSpeed_TimerCallback_right();
	
	Motor_set_pid(300);
}


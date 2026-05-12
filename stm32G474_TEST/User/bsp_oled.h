/**1
 * @file bsp_oled.h
 * @brief a
 *
 * This file contains function declarations and necessary includes
 * for the bsp_oled module.
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

#ifndef BSP_OLED_H
#define BSP_OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_Part(uint8_t Line, uint8_t start, uint8_t end);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowWord(uint8_t Line, uint8_t Column, uint8_t Chinese);
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t *Chinese, uint8_t Length);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Init(void);
	
#ifdef __cplusplus
}
#endif

#endif // BSP_OLED_H

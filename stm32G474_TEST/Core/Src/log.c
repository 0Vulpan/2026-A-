/**
 * @file log.c
 * @brief 使用串口实现日志打印
 *
 * This file contains implementation of initialization and processing
 * functions for the log module.
 *
 * @author Xue
 * @version 1.0.0
 * @date 2025-07-02
 *
 * @Copyright (c) 2025, Xue Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2025-07-02	Xue	Initial version
 */

#include "log.h"

/*============================ INCLUDES ======================================*/
#include "usart.h"
#include <string.h>
#include <stdarg.h>
/*============================ MACROS ========================================*/
#define LOG_BUF_SIZE 1024
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
extern UART_HandleTypeDef huart1;
/*============================ LOCAL VARIABLES ===============================*/
static uint8_t log_buf[LOG_BUF_SIZE];
static volatile uint16_t log_head = 0;
static volatile uint16_t log_tail = 0;
static volatile uint8_t dma_busy = 0;
/*============================ PROTOTYPES ====================================*/

/*============================ INTERNAL IMPLEMENTATION =======================*/
static uint16_t log_buf_free_space(void) {
    if (log_tail <= log_head)
        return LOG_BUF_SIZE - (log_head - log_tail) - 1;
    else
        return log_tail - log_head - 1;
}

static uint16_t log_buf_write(const uint8_t *data, uint16_t len) {
    uint16_t free_space = log_buf_free_space();
    if (len > free_space)
        len = free_space;

    for (uint16_t i = 0; i < len; i++) {
        log_buf[log_head++] = data[i];
        if (log_head >= LOG_BUF_SIZE)
            log_head = 0;
    }
    return len;
}

static void log_start_dma_tx(void) {
    if (dma_busy || log_tail == log_head)
        return;

    dma_busy = 1;
    uint16_t len = (log_head > log_tail) ? (log_head - log_tail) : (LOG_BUF_SIZE - log_tail);
    HAL_UART_Transmit_DMA(&huart1, &log_buf[log_tail], len);
}


/*============================ EXTERNAL IMPLEMENTATION =======================*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        log_tail = (log_tail + huart->TxXferSize) % LOG_BUF_SIZE;
        dma_busy = 0;
        log_start_dma_tx();
    }

}

void log_printf(const char *format, ...) {
    char temp_buf[256];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(temp_buf, sizeof(temp_buf), format, args);
    va_end(args);

    if (len > 0 && log_buf_write((uint8_t *)temp_buf, len) > 0) {
        log_start_dma_tx();
    }
}

void log_init(void) {
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"", 0);  // 初始化 DMA 状态
	    // 初始化时输出日志标题头
    log_printf("+--------------------------------------+\r\n");
    log_printf("|        LOG SYSTEM INITIALIZED        |\r\n");
    log_printf("|        Build: %s %s   |\r\n", __DATE__, __TIME__);
    log_printf("+--------------------------------------+\r\n");	
}


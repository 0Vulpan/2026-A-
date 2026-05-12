/**
 * @file log.h
 * @brief 使用串口实现日志打印
 *
 * This file contains function declarations and necessary includes
 * for the log module.
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

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================ INCLUDES ======================================*/
#include "stm32g4xx.h"                  // Device header
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
void log_init(void);
void log_printf(const char *format, ...);
void log_printf_ex(const char *level_color, const char *level_tag, const char *file, int line, const char *format, ...);

#define LOG_COLOR_RESET   "\033[0m"
#define LOG_COLOR_RED     "\033[1;31m"
#define LOG_COLOR_YELLOW  "\033[1;33m"
#define LOG_COLOR_GREEN   "\033[1;32m"

#define LOG_I(fmt, ...) log_printf(LOG_COLOR_GREEN "[I] " fmt LOG_COLOR_RESET "\r\n", ##__VA_ARGS__)
#define LOG_W(fmt, ...) log_printf(LOG_COLOR_YELLOW "[W] " fmt LOG_COLOR_RESET "\r\n", ##__VA_ARGS__)
#define LOG_E(fmt, ...) log_printf(LOG_COLOR_RED "[E] " fmt LOG_COLOR_RESET "\r\n", ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOG_H

/**1
 * @file gw_iic.h
 * @brief a
 *
 * This file contains function declarations and necessary includes
 * for the gw_iic module.
 *
 * @author chy
 * @version 1.0.0
 * @date 2026-04-08
 *
 * @Copyright (c) 2026, PLKJ Development Team, All rights reserved.
 *
 * @par Change Logs:
 * Date			Author		Notes
 * 2026-04-08	chy	Initial version
 */

#ifndef GW_IIC_H
#define GW_IIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
/*============================ INCLUDES ======================================*/

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERNAL IMPLEMENTATION =======================*/
void gw_output(void);
void gw_init(void);

#ifdef __cplusplus
}
#endif

#endif // GW_IIC_H

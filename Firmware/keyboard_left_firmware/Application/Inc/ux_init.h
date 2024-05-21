/*
 * ux_init.h
 *
 *  Created on: May 20, 2024
 *      Author: bens1
 */

#ifndef INC_UX_INIT_H_
#define INC_UX_INIT_H_

#include "ux_api.h"

// TODO: Write prebuild python script that removes USBX/App/, and includes this file in app_azure_rtos.c. Also replace app_threadx.c with functions from app_init.c
UINT MX_USBX_Device_Init(VOID *memory_ptr);

#endif /* INC_UX_INIT_H_ */

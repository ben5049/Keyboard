/*
 * init.h
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 *
 *  Description: Various initialisation functions that need to be called from generated code.
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_

#include "ux_api.h"

/*
    @ACTION: USER_CODE_INSERT
    @FILE: "Core/Src/app_threadx.c"
    @SECTION: "Includes"
    @INSERT: "#include "init.h""
    @SECTION: "App_ThreadX_Init"
    @INSERT: "create_threads();"
 */
void create_threads(void);

/*
	@ACTION: USER_CODE_INSERT
    @FILE: "Core/Src/app_threadx.c"
    @SECTION: "Includes"
    @INSERT: "#include "init.h""
	@SECTION: "App_ThreadX_Init"
    @INSERT: "create_queues();"
 */
void create_queues(void);

/*
    @ACTION: USER_CODE_INSERT
    @FILE: "USBX/App/app_usbx_device.c"
    @SECTION: "Includes"
    @INSERT: "#include "init.h""
    @SECTION: "MX_USBX_Device_Init"
    @INSERT: "usb_init(memory_ptr);"
*/
UINT usb_init(VOID *memory_ptr);

#endif /* INC_INIT_H_ */

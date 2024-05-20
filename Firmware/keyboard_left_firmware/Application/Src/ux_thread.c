/*
 * ux_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "ux_threads.h"
#include "ux_dcd_stm32.h"

#include "usb_drd_fs.h"

void ux_thread(uint32_t thread_input){

	MX_USB_DRD_FS_PCD_Init();

	/*
	 * TODO: REDO THIS
	 *
	 * Configure packet buffers to receive end point data
	 *
	 * PMA layout:
	 * 	BTABLE:          0x00 -  0x17 (24 bytes)
	 * 	EP0 OUT (0x00):  0x18 -  0x57 (64 bytes)
	 * 	EP0 IN  (0x80):  0x58 -  0x97 (64 bytes)
	 * 	EP1 OUT (0x01):  0x98 -  0xD7 (64 bytes)
	 * 	EP1 IN  (0x81):  0xD8 - 0x1D7 (256 bytes)
	 * 	EP2 OUT (0x02): 0x1D8 - 0x217 (64 bytes)
	 * 	EP2 IN  (0x82): 0x218 - 0x257 (64 bytes)
	 *
	 * BTABLE uses the first 20 bytes of the PMA when the USB peripheral is configured
	 * for 3 end points (EP0, EP1, EP2). For more info, see RM0440 p2008.
	 */

	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00, PCD_SNG_BUF, 0x14);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80, PCD_SNG_BUF, 0x54);

	/* EndPoint_Configuration_CDC */
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x81, PCD_SNG_BUF, 0x94);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x01, PCD_SNG_BUF, 0xD4);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x82, PCD_SNG_BUF, 0x114);


	ux_dcd_stm32_initialize((ULONG)USB_DRD_FS, (ULONG)&hpcd_USB_DRD_FS);

	HAL_PCD_Start(&hpcd_USB_DRD_FS);
}


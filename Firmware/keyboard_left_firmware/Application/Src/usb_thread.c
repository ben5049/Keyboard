/*
 * usb_thread.c
 *
 *  Created on: May 26, 2024
 *      Author: bens1
 */

#include "usb_threads.h"
#include "USB2533.h"
#include "i2c.h"
#include "ux_dcd_stm32.h"
#include "usb_drd_fs.h"
#include "ux_device_descriptors.h"

USB2533_HandleTypeDef usb2533;

/* PFP */
void STM32_USB_Init(void);


void usb_thread(uint32_t thread_input){

	HAL_StatusTypeDef status;

	USB2533_Init(&usb2533, &hi2c2);
	STM32_USB_Init();

	while (1){
		tx_thread_sleep(100);

	}
}


void STM32_USB_Init(){

	MX_USB_DRD_FS_PCD_Init();

	/*
	 * TODO: REDO THIS
	 *
	 * Configure packet buffers to receive end point data
	 *
	 * Example PMA layout:
	 * 	BTABLE:          0x00 -  0x17 (24 bytes)
	 * 	EP0 OUT (0x00):  0x18 -  0x57 (64 bytes)
	 * 	EP0 IN  (0x80):  0x58 -  0x97 (64 bytes)
	 * 	EP1 OUT (0x01):  0x98 -  0xD7 (64 bytes)
	 * 	EP1 IN  (0x81):  0xD8 - 0x1D7 (256 bytes)
	 * 	EP2 OUT (0x02): 0x1D8 - 0x217 (64 bytes)
	 * 	EP2 IN  (0x82): 0x218 - 0x257 (64 bytes)
	 *
	 * BTABLE uses the first 20 bytes of the PMA when the USB peripheral is configured
	 * for 3 end points (EP0, EP1, EP2). For more info, see RM0440 p1268.
	 */

	/* Configure control endpoints */
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00, PCD_SNG_BUF, 0x40);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80, PCD_SNG_BUF, 0x80);

	/* Configure class endpoints */
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, USBD_MSC_EPOUT_ADDR, PCD_SNG_BUF, 0xC0);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, USBD_MSC_EPIN_ADDR, PCD_SNG_BUF, 0x100);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, USBD_HID_MOUSE_EPIN_ADDR, PCD_SNG_BUF, 0x140);
	HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, USBD_HID_KEYBOARD_EPIN_ADDR, PCD_SNG_BUF, 0x180);

	ux_dcd_stm32_initialize((ULONG)USB_DRD_FS, (ULONG)&hpcd_USB_DRD_FS);

	HAL_PCD_Start(&hpcd_USB_DRD_FS);
}

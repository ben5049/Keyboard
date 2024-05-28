/*
 * hid_keyboard_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include <key.h>
#include "usb_threads.h"
#include "threads.h"

UX_SLAVE_CLASS_HID *hid_keyboard;


/**
 * @brief  USBD_HID_Keyboard_Activate
 *         This function is called when insertion of a HID Keyboard device.
 * @param  hid_instance: Pointer to the hid class instance.
 * @retval none
 */
VOID USBD_HID_Keyboard_Activate(VOID *hid_instance)
{
	/* USER CODE BEGIN USBD_HID_Keyboard_Activate */

	/* Save the HID keyboard instance */
	hid_keyboard = (UX_SLAVE_CLASS_HID*) hid_instance;

	/* USER CODE END USBD_HID_Keyboard_Activate */

	return;
}

/**
 * @brief  USBD_HID_Keyboard_Deactivate
 *         This function is called when extraction of a HID Keyboard device.
 * @param  hid_instance: Pointer to the hid class instance.
 * @retval none
 */
VOID USBD_HID_Keyboard_Deactivate(VOID *hid_instance)
{
	/* USER CODE BEGIN USBD_HID_Keyboard_Deactivate */
	UX_PARAMETER_NOT_USED(hid_instance);

	/* Reset the HID mouse instance */
	hid_keyboard = UX_NULL;

	/* USER CODE END USBD_HID_Keyboard_Deactivate */

	return;
}

/**
 * @brief  USBD_HID_Keyboard_SetReport
 *         This function is invoked when the host sends a HID SET_REPORT
 *         to the application over Endpoint 0.
 * @param  hid_instance: Pointer to the hid class instance.
 * @param  hid_event: Pointer to structure of the hid event.
 * @retval status
 */
UINT USBD_HID_Keyboard_SetReport(UX_SLAVE_CLASS_HID *hid_instance,
		UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
	UINT status = UX_SUCCESS;

	/* USER CODE BEGIN USBD_HID_Keyboard_SetReport */
	UX_PARAMETER_NOT_USED(hid_instance);
	UX_PARAMETER_NOT_USED(hid_event);
	/* USER CODE END USBD_HID_Keyboard_SetReport */

	return status;
}

/**
 * @brief  USBD_HID_Keyboard_GetReport
 *         This function is invoked when host is requesting event through
 *         control GET_REPORT request.
 * @param  hid_instance: Pointer to the hid class instance.
 * @param  hid_event: Pointer to structure of the hid event.
 * @retval status
 */
UINT USBD_HID_Keyboard_GetReport(UX_SLAVE_CLASS_HID *hid_instance,
		UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
	UINT status = UX_SUCCESS;

	/* USER CODE BEGIN USBD_HID_Keyboard_GetReport */
	UX_PARAMETER_NOT_USED(hid_instance);
	UX_PARAMETER_NOT_USED(hid_event);
	/* USER CODE END USBD_HID_Keyboard_GetReport */

	return status;
}

/* USER CODE BEGIN 1 */

/**
 * @brief  Function implementing usbx_hid_keyboard_thread_entry.
 * @param  thread_input: not used
 * @retval none
 */
void hid_keyboard_thread(uint32_t thread_input){
	UX_SLAVE_DEVICE *device;
	UX_SLAVE_CLASS_HID_EVENT hid_event;

	uint8_t modifier = 0;
	keyEvent_TypeDef key_event;


	TX_PARAMETER_NOT_USED(thread_input);

	/* Get the pointer to the device */
	device = &_ux_system_slave -> ux_system_slave_device;

	/* Reset the HID event structure */
	ux_utility_memory_set(&hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

	while (1)
	{
		/* Check if the device state already configured */


		if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (hid_keyboard != UX_NULL))
		{

			tx_queue_receive(&keyboard_queue_ptr, &key_event, TX_WAIT_FOREVER);

			system_modifiers_get(&hid_event.ux_device_class_hid_event_buffer[0]);

			if (key_event.state == KEY_PRESSED){

				hid_event.ux_device_class_hid_event_length = 8;

//				/* This byte is a modifier byte */
//				hid_event.ux_device_class_hid_event_buffer[0] = modifier;

				/* This byte is reserved */
				hid_event.ux_device_class_hid_event_buffer[1] = 0;

				/* Update key button byte */
				hid_event.ux_device_class_hid_event_buffer[2] = key_event.key;

				/* Send keyboard event */
				ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			}


			else if (key_event.state == KEY_RELEASED){

				hid_event.ux_device_class_hid_event_length = 8;

//				/* This byte is a modifier byte */
//				hid_event.ux_device_class_hid_event_buffer[0] = modifier;

				/* This byte is reserved */
				hid_event.ux_device_class_hid_event_buffer[1] = 0;

				/* Update key button byte */
				hid_event.ux_device_class_hid_event_buffer[2] = 0;

				/* Send keyboard event */
				ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			}
		}

		else
		{
			/* Sleep thread for 10ms */
			tx_thread_sleep(MS_TO_TICK(10));
		}
	}
}

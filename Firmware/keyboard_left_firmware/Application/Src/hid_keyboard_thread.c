/*
 * hid_keyboard_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "usb_threads.h"

UX_SLAVE_CLASS_HID *hid_keyboard;

static VOID GetKeyData(UX_SLAVE_CLASS_HID_EVENT *hid_event);


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

	GPIO_PinState current_state, previous_state;


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
			/* Sleep Thread for 20ms */
			tx_thread_sleep(MS_TO_TICK(20));

			current_state = HAL_GPIO_ReadPin(KEY_A6_GPIO_Port, KEY_A6_Pin);

			if (current_state == GPIO_PIN_RESET && previous_state == GPIO_PIN_SET){

				hid_event.ux_device_class_hid_event_length = 8;

				/* This byte is a modifier byte */
				hid_event.ux_device_class_hid_event_buffer[0] = 0;

				/* This byte is reserved */
				hid_event.ux_device_class_hid_event_buffer[1] = 0;

				/* Update key button byte */
				hid_event.ux_device_class_hid_event_buffer[2] = 34;

				/* Send keyboard event */
				ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			}


			if (current_state == GPIO_PIN_SET && previous_state == GPIO_PIN_RESET){

				hid_event.ux_device_class_hid_event_length = 8;

				/* This byte is a modifier byte */
				hid_event.ux_device_class_hid_event_buffer[0] = 0;

				/* This byte is reserved */
				hid_event.ux_device_class_hid_event_buffer[1] = 0;

				/* Update key button byte */
				hid_event.ux_device_class_hid_event_buffer[2] = 0;

				/* Send keyboard event */
				ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			}

			previous_state = current_state;

			//      /* Check if user button is pressed */
			//      if (User_Button_State)
			//      {
			//        /* Get the key button */
			//        GetKeyData(&hid_event);
			//
			//        /* Send keyboard event */
			//        ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			//
			//        /* Next event has the key button depressed */
			//        hid_event.ux_device_class_hid_event_buffer[2] = 0;
			//
			//        /* Set hid envent length to 8 */
			//        hid_event.ux_device_class_hid_event_length = 8;
			//
			//        /* Send keyboard event */
			//        ux_device_class_hid_event_set(hid_keyboard, &hid_event);
			//
			//        /* Reset User Button state */
			//        User_Button_State = 0;
			//      }
		}
		else
		{
			/* Sleep thread for 10ms */
			tx_thread_sleep(MS_TO_TICK(10));
		}
	}
}

/**
 * @brief  GetKeyData
 *         Gets Pointer Data.
 * @param  hid_event: Pointer to hid event buffer.
 * @retval none
 */
static VOID GetKeyData(UX_SLAVE_CLASS_HID_EVENT *hid_event)
{

	uint32_t key_button = 0x05;

	/* Set hid event length to 8 */
	hid_event->ux_device_class_hid_event_length = 8;

	/* This byte is a modifier byte */
	hid_event->ux_device_class_hid_event_buffer[0] = 0;

	/* This byte is reserved */
	hid_event->ux_device_class_hid_event_buffer[1] = 0;

	/* Update key button byte */
	hid_event->ux_device_class_hid_event_buffer[2] = key_button;

	/* Increment counter */
	//  key_button++;
}

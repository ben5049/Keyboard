	/*
 * usb_init.c
 *
 *  Created on: May 20, 2024
 *      Author: bens1
 */

#include "usb_threads.h"
#include "usb_init.h"
#include "app_azure_rtos.h"
#include "stdint.h"

#include "ux_device_descriptors.h"




TX_THREAD usb_thread_ptr;
TX_THREAD hid_mouse_thread_ptr;
TX_THREAD hid_keyboard_thread_ptr;
TX_THREAD msc_thread_ptr;


uint32_t hid_mouse_interface_number;
uint32_t hid_mouse_configuration_number;
UX_SLAVE_CLASS_HID_PARAMETER hid_mouse_parameter;
ULONG hid_keyboard_interface_number;
ULONG hid_keyboard_configuration_number;
UX_SLAVE_CLASS_HID_PARAMETER hid_keyboard_parameter;
ULONG storage_interface_number;
ULONG storage_configuration_number;
UX_SLAVE_CLASS_STORAGE_PARAMETER storage_parameter;

UINT usb_init(VOID *memory_ptr)
{
	UINT ret = UX_SUCCESS;
	TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
	UCHAR *device_framework_high_speed;
	UCHAR *device_framework_full_speed;
	ULONG device_framework_hs_length;
	ULONG device_framework_fs_length;
	ULONG string_framework_length;
	ULONG language_id_framework_length;
	UCHAR *string_framework;
	UCHAR *language_id_framework;
	UCHAR *pointer;

	(void)byte_pool;

	/* Allocate the stack for USBX Memory */
	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
	{
		/* USER CODE BEGIN USBX_ALLOCATE_STACK_ERORR */
		return TX_POOL_ERROR;
		/* USER CODE END USBX_ALLOCATE_STACK_ERORR */
	}

	/* Initialise USBX Memory */
	if (ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
	{
		/* USER CODE BEGIN USBX_SYSTEM_INITIALIZE_ERORR */
		return UX_ERROR;
		/* USER CODE END USBX_SYSTEM_INITIALIZE_ERORR */
	}

	/* Get Device Framework High Speed and get the length */
	device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED, &device_framework_hs_length);

	/* Get Device Framework Full Speed and get the length */
	device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED, &device_framework_fs_length);

	/* Get String Framework and get the length */
	string_framework = USBD_Get_String_Framework(&string_framework_length);

	/* Get Language Id Framework and get the length */
	language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

	/* Install the device portion of USBX */
	if (ux_device_stack_initialize(device_framework_high_speed,
			device_framework_hs_length,
			device_framework_full_speed,
			device_framework_fs_length,
			string_framework,
			string_framework_length,
			language_id_framework,
			language_id_framework_length,
			UX_NULL) != UX_SUCCESS)
	{
		return UX_ERROR;
	}

	/* Initialize the hid mouse class parameters for the device */
	hid_mouse_parameter.ux_slave_class_hid_instance_activate         = USBD_HID_Mouse_Activate;
	hid_mouse_parameter.ux_slave_class_hid_instance_deactivate       = USBD_HID_Mouse_Deactivate;
	hid_mouse_parameter.ux_device_class_hid_parameter_report_address = USBD_HID_ReportDesc(INTERFACE_HID_MOUSE);
	hid_mouse_parameter.ux_device_class_hid_parameter_report_length  = USBD_HID_ReportDesc_length(INTERFACE_HID_MOUSE);
	hid_mouse_parameter.ux_device_class_hid_parameter_report_id      = UX_FALSE;
	hid_mouse_parameter.ux_device_class_hid_parameter_callback       = USBD_HID_Mouse_SetReport;
	hid_mouse_parameter.ux_device_class_hid_parameter_get_callback   = USBD_HID_Mouse_GetReport;

	/* Get hid mouse configuration number */
	hid_mouse_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_HID, INTERFACE_HID_MOUSE);

	/* Find hid mouse interface number */
	hid_mouse_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_HID, INTERFACE_HID_MOUSE);

	/* Initialize the device hid Mouse class */
	if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
			ux_device_class_hid_entry,
			hid_mouse_configuration_number,
			hid_mouse_interface_number,
			&hid_mouse_parameter) != UX_SUCCESS)
	{
		/* USER CODE BEGIN USBX_DEVICE_HID_MOUSE_REGISTER_ERORR */
		return UX_ERROR;
		/* USER CODE END USBX_DEVICE_HID_MOUSE_REGISTER_ERORR */
	}

	/* Initialize the hid keyboard class parameters for the device */
	hid_keyboard_parameter.ux_slave_class_hid_instance_activate         = USBD_HID_Keyboard_Activate;
	hid_keyboard_parameter.ux_slave_class_hid_instance_deactivate       = USBD_HID_Keyboard_Deactivate;
	hid_keyboard_parameter.ux_device_class_hid_parameter_report_address = USBD_HID_ReportDesc(INTERFACE_HID_KEYBOARD);
	hid_keyboard_parameter.ux_device_class_hid_parameter_report_length  = USBD_HID_ReportDesc_length(INTERFACE_HID_KEYBOARD);
	hid_keyboard_parameter.ux_device_class_hid_parameter_report_id      = UX_FALSE;
	hid_keyboard_parameter.ux_device_class_hid_parameter_callback       = USBD_HID_Keyboard_SetReport;
	hid_keyboard_parameter.ux_device_class_hid_parameter_get_callback   = USBD_HID_Keyboard_GetReport;

	/* USER CODE BEGIN HID_KEYBOARD_PARAMETER */

	/* USER CODE END HID_KEYBOARD_PARAMETER */

	/* Get hid keyboard configuration number */
	hid_keyboard_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_HID, INTERFACE_HID_KEYBOARD);

	/* Find hid keyboard interface number */
	hid_keyboard_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_HID, INTERFACE_HID_KEYBOARD);

	/* Initialize the device hid keyboard class */
	if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
			ux_device_class_hid_entry,
			hid_keyboard_configuration_number,
			hid_keyboard_interface_number,
			&hid_keyboard_parameter) != UX_SUCCESS)
	{
		return UX_ERROR;
	}

	/* Initialise the storage class parameters for the device */
	storage_parameter.ux_slave_class_storage_instance_activate   = USBD_STORAGE_Activate;
	storage_parameter.ux_slave_class_storage_instance_deactivate = USBD_STORAGE_Deactivate;

	/* Store the number of LUN in this device storage instance */
	storage_parameter.ux_slave_class_storage_parameter_number_lun = STORAGE_NUMBER_LUN;

	/* Initialise the storage class parameters for reading/writing to the Flash Disk */
	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_last_lba = USBD_STORAGE_GetMediaLastLba();

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_block_length = USBD_STORAGE_GetMediaBlocklength();

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_type = 0;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_removable_flag = STORAGE_REMOVABLE_FLAG;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_read_only_flag = STORAGE_READ_ONLY;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_read = USBD_STORAGE_Read;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_write = USBD_STORAGE_Write;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_flush = USBD_STORAGE_Flush;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_status = USBD_STORAGE_Status;

	storage_parameter.ux_slave_class_storage_parameter_lun[0].
	ux_slave_class_storage_media_notification = USBD_STORAGE_Notification;

	/* Get storage configuration number */
	storage_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_MSC, 0);

	/* Find storage interface number */
	storage_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_MSC, 0);

	/* Initialise the device storage class */
	if (ux_device_stack_class_register(_ux_system_slave_class_storage_name,
			ux_device_class_storage_entry,
			storage_configuration_number,
			storage_interface_number,
			&storage_parameter) != UX_SUCCESS)
	{
		return UX_ERROR;
	}

	/* Allocate the stack for device application main thread */
	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_THREAD_STACK_SIZE,
			TX_NO_WAIT) != TX_SUCCESS)
	{
		return TX_POOL_ERROR;
	}

	/* Create the device application main thread */
	if (tx_thread_create(&usb_thread_ptr, USB_THREAD_NAME, usb_thread,
			0, pointer, UX_THREAD_STACK_SIZE, USB_THREAD_PRIORITY,
			USB_THREAD_PRIORITY, 1,
			TX_AUTO_START) != TX_SUCCESS)
	{
		return TX_THREAD_ERROR;
	}

	/* Allocate the stack for usbx hid mouse thread */
	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_HID_MOUSE_THREAD_STACK_SIZE, TX_NO_WAIT) != UX_SUCCESS)
	{
		return TX_POOL_ERROR;
	}

	/* Create the usbx hid mouse thread */
	if (tx_thread_create(&hid_mouse_thread_ptr, USBX_HID_MOUSE_THREAD_NAME,
			hid_mouse_thread, 0, pointer, USBX_HID_MOUSE_THREAD_STACK_SIZE,
			USBX_HID_MOUSE_THREAD_PRIORITY, USBX_HID_MOUSE_THREAD_PRIORITY, 1, TX_AUTO_START) != UX_SUCCESS)
	{
		return TX_THREAD_ERROR;
	}

	/* Allocate the stack for usbx hid keyboard thread */
	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_HID_KEYBOARD_THREAD_STACK_SIZE, TX_NO_WAIT) != UX_SUCCESS)
	{
		return TX_POOL_ERROR;
	}

	/* Create the usbx hid keyboard thread */
	if (tx_thread_create(&hid_keyboard_thread_ptr, USBX_HID_KEYBOARD_THREAD_NAME,
			hid_keyboard_thread, 0, pointer, USBX_HID_KEYBOARD_THREAD_STACK_SIZE,
			USBX_HID_KEYBOARD_THREAD_PRIORITY, USBX_HID_KEYBOARD_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START) != UX_SUCCESS)
	{
		return TX_THREAD_ERROR;
	}

	/* Allocate the stack for usbx msc thread */
	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_MSC_THREAD_STACK_SIZE, TX_NO_WAIT) != UX_SUCCESS)
	{
		return TX_POOL_ERROR;
	}

	/* Create the usbx msc thread */
	if (tx_thread_create(&msc_thread_ptr, USBX_MSC_THREAD_NAME,
			msc_thread, 0, pointer, USBX_MSC_THREAD_STACK_SIZE,
			USBX_MSC_THREAD_PRIORITY, USBX_MSC_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START) != UX_SUCCESS)
	{
		return TX_THREAD_ERROR;
	}

	return ret;
}



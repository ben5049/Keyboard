/*
 * ux_threads.h
 *
 *  Created on: May 20, 2024
 *      Author: bens1
 */

#ifndef INC_UX_THREADS_H_
#define INC_UX_THREADS_H_

#include "stdint.h"
#include "main.h"
#include "app_azure_rtos.h"

#include "ux_api.h"
#include "ux_device_class_hid.h"
#include "ux_device_class_storage.h"

#define USBX_MEMORY_SIZE (15104)

//#define UX_THREAD_STACK_SIZE 1024
#define UX_THREAD_NAME "usbx thread"
#define UX_THREAD_PRIORITY 10

#define USBX_HID_MOUSE_THREAD_STACK_SIZE 1024
#define USBX_HID_MOUSE_THREAD_NAME "mouse thread"
#define USBX_HID_MOUSE_THREAD_PRIORITY 20

#define USBX_HID_KEYBOARD_THREAD_STACK_SIZE 1024
#define USBX_HID_KEYBOARD_THREAD_NAME "keyboard thread"
#define USBX_HID_KEYBOARD_THREAD_PRIORITY 20

#define USBX_MSC_THREAD_STACK_SIZE 1024
#define USBX_MSC_THREAD_NAME "keyboard thread"
#define USBX_MSC_THREAD_PRIORITY 20

#define STORAGE_NUMBER_LUN   1
#define STORAGE_REMOVABLE_FLAG   0x80U
#define STORAGE_READ_ONLY    UX_FALSE

void ux_thread(uint32_t thread_input);
void hid_mouse_thread(uint32_t thread_input);
void hid_keyboard_thread(uint32_t thread_input);
void msc_thread(uint32_t thread_input);



VOID USBD_HID_Mouse_Activate(VOID *hid_instance);
VOID USBD_HID_Mouse_Deactivate(VOID *hid_instance);
UINT USBD_HID_Mouse_SetReport(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
UINT USBD_HID_Mouse_GetReport(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);

VOID USBD_HID_Keyboard_Activate(VOID *hid_instance);
VOID USBD_HID_Keyboard_Deactivate(VOID *hid_instance);
UINT USBD_HID_Keyboard_SetReport(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
UINT USBD_HID_Keyboard_GetReport(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);


VOID USBD_STORAGE_Activate(VOID *storage_instance);
VOID USBD_STORAGE_Deactivate(VOID *storage_instance);
UINT USBD_STORAGE_Read(VOID *storage_instance, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT USBD_STORAGE_Write(VOID *storage_instance, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT USBD_STORAGE_Flush(VOID *storage_instance, ULONG lun, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT USBD_STORAGE_Status(VOID *storage_instance, ULONG lun, ULONG media_id, ULONG *media_status);
UINT USBD_STORAGE_Notification(VOID *storage_instance, ULONG lun, ULONG media_id, ULONG notification_class, UCHAR **media_notification, ULONG *media_notification_length);
ULONG USBD_STORAGE_GetMediaLastLba(VOID);
ULONG USBD_STORAGE_GetMediaBlocklength(VOID);

#endif /* INC_UX_THREADS_H_ */

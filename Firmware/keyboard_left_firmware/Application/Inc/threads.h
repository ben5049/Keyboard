/*
 * threads.h
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#ifndef THREADS_H_
#define THREADS_H_

#include "stdint.h"
#include "main.h"
#include "app_azure_rtos.h"

/* Thread parameters */
#define SCAN_KEYS_THREAD_STACK_SIZE 1024
#define SCAN_KEYS_THREAD_NAME "scan keys"
#define SCAN_KEYS_THREAD_PRIORITY 15

#define USBX_APP_THREAD_STACK_SIZE 1024
#define USBX_APP_THREAD_PRIORITY 10

#define USBX_HID_MOUSE_THREAD_STACK_SIZE 1024
#define USBX_HID_KEYBOARD_THREAD_PRIORITY 20

#define USBX_HID_KEYBOARD_THREAD_STACK_SIZE 1024
#define USBX_HID_KEYBOARD_THREAD_PRIORITY 20

#define USBX_MEMORY_SIZE                             (15104)

/* Thread functions */
void scan_keys_thread(uint32_t initial_input);

#endif /* THREADS_H_ */

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
#include "config.h"

/* Thread parameters */
#define SCAN_KEYS_THREAD_STACK_SIZE 1024
#define SCAN_KEYS_THREAD_NAME "scan keys"
#define SCAN_KEYS_THREAD_PRIORITY 15

/* Thread functions */
void scan_keys_thread(uint32_t initial_input);

/* Queue parameters */
#define KEYBOARD_QUEUE_STACK_SIZE 128
#define KEYBOARD_QUEUE_NAME "Keyboard_queue"

extern TX_QUEUE keyboard_queue_ptr;



#endif /* THREADS_H_ */

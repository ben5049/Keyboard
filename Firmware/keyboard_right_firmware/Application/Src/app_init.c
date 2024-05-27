/*
 * app_init.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "app_azure_rtos.h"
#include "stdint.h"

#include "app_init.h"
#include "threads.h"

/* Create thread stacks */
uint8_t scan_keys_thread_stack[SCAN_KEYS_THREAD_STACK_SIZE];

/* Create thread pointers */
TX_THREAD scan_keys_thread_ptr;

/* Create queue stacks */
uint8_t keyboard_queue_stack[KEYBOARD_QUEUE_STACK_SIZE];

/* Create queue pointers */
TX_QUEUE keyboard_queue_ptr;

/* Place this function under "USER CODE BEGIN App_ThreadX_Init" */
void create_threads(){
	tx_thread_create(&scan_keys_thread_ptr, SCAN_KEYS_THREAD_NAME, scan_keys_thread, 0x1234, scan_keys_thread_stack, SCAN_KEYS_THREAD_STACK_SIZE, SCAN_KEYS_THREAD_PRIORITY, SCAN_KEYS_THREAD_PRIORITY, 1, TX_AUTO_START);
}

void create_queues(){
	tx_queue_create(&keyboard_queue_ptr, KEYBOARD_QUEUE_NAME, 1, keyboard_queue_stack, 128);
}



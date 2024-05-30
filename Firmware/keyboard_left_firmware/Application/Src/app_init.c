/*
 * app_init.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "init.h"
#include "app_azure_rtos.h"
#include "stdint.h"
#include "threads.h"

/* Create thread stacks */
uint8_t scan_keys_thread_stack[SCAN_KEYS_THREAD_STACK_SIZE];

/* Create thread pointers */
TX_THREAD scan_keys_thread_ptr;

/* Create queue stacks */
uint8_t key_event_queue_ptr_stack[KEY_EVENT_QUEUE_STACK_SIZE];
uint8_t mouse_queue_stack[MOUSE_QUEUE_STACK_SIZE];

/* Create queue pointers */
TX_QUEUE key_event_queue_ptr;
TX_QUEUE mouse_queue_ptr;

/* Place this function under "USER CODE BEGIN App_ThreadX_Init" */
void create_threads(){
	tx_thread_create(&scan_keys_thread_ptr, SCAN_KEYS_THREAD_NAME, scan_keys_thread, 0x1234, scan_keys_thread_stack, SCAN_KEYS_THREAD_STACK_SIZE, SCAN_KEYS_THREAD_PRIORITY, SCAN_KEYS_THREAD_PRIORITY, 1, TX_AUTO_START);
}

void create_queues(){
	tx_queue_create(&key_event_queue_ptr, KEY_EVENT_QUEUE_NAME, 1, key_event_queue_ptr_stack, 128);
	tx_queue_create(&mouse_queue_ptr, MOUSE_QUEUE_NAME, 1, mouse_queue_stack, 128);
}



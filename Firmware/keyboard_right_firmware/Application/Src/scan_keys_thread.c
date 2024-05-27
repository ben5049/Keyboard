/*
 * scan_keys_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */


#include "threads.h"
#include "keys.h"

key_HandleTypeDef keys[NUMBER_OF_KEYS];

void scan_keys_thread(uint32_t initial_input){

	key_init(&keys[0], KEY_NAME_6, KEY_A7_GPIO_Port, KEY_A7_Pin, &keyboard_queue_ptr);
	key_init(&keys[1], KEY_NAME_7, KEY_A8_GPIO_Port, KEY_A8_Pin, &keyboard_queue_ptr);
	key_init(&keys[2], KEY_NAME_8, KEY_A9_GPIO_Port, KEY_A9_Pin, &keyboard_queue_ptr);
	key_init(&keys[3], KEY_NAME_9, KEY_A10_GPIO_Port, KEY_A10_Pin, &keyboard_queue_ptr);
	key_init(&keys[4], KEY_NAME_0, KEY_A11_GPIO_Port, KEY_A11_Pin, &keyboard_queue_ptr);
	key_init(&keys[5], KEY_NAME_Y, KEY_B7_GPIO_Port, KEY_B7_Pin, &keyboard_queue_ptr);
	key_init(&keys[6], KEY_NAME_U, KEY_B8_GPIO_Port, KEY_B8_Pin, &keyboard_queue_ptr);
	key_init(&keys[7], KEY_NAME_I, KEY_B9_GPIO_Port, KEY_B9_Pin, &keyboard_queue_ptr);
	key_init(&keys[8], KEY_NAME_O, KEY_B10_GPIO_Port, KEY_B10_Pin, &keyboard_queue_ptr);
	key_init(&keys[9], KEY_NAME_P, KEY_B11_GPIO_Port, KEY_B11_Pin, &keyboard_queue_ptr);
	key_init(&keys[10], KEY_NAME_H, KEY_C7_GPIO_Port, KEY_C7_Pin, &keyboard_queue_ptr);
	key_init(&keys[11], KEY_NAME_J, KEY_C8_GPIO_Port, KEY_C8_Pin, &keyboard_queue_ptr);
	key_init(&keys[12], KEY_NAME_K, KEY_C9_GPIO_Port, KEY_C9_Pin, &keyboard_queue_ptr);
	key_init(&keys[13], KEY_NAME_L, KEY_C10_GPIO_Port, KEY_C10_Pin, &keyboard_queue_ptr);
	key_init(&keys[14], KEY_NAME_N, KEY_D7_GPIO_Port, KEY_D7_Pin, &keyboard_queue_ptr);
	key_init(&keys[15], KEY_NAME_M, KEY_D8_GPIO_Port, KEY_D8_Pin, &keyboard_queue_ptr);
	key_init(&keys[16], KEY_MOD_LCTRL, KEY_E4_GPIO_Port, KEY_E4_Pin, &keyboard_queue_ptr);
	key_init(&keys[17], KEY_NAME_ENTER, KEY_E5_GPIO_Port, KEY_E5_Pin, &keyboard_queue_ptr);


	while (1){

		for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
			key_update(&keys[i]);
		}


		tx_thread_sleep(10); // TODO: Remove sleep and do proper timing
	}
}

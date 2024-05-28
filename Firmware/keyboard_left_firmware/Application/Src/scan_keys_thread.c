/*
 * scan_keys_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */


#include "threads.h"
#include "keys.h"

static key_HandleTypeDef keys[NUMBER_OF_KEYS];

void scan_keys_thread(uint32_t initial_input){

	keys[0].pin = KEY_A2_Pin;
	keys[0].port = KEY_A2_GPIO_Port;
	keys[0].layers[0].key_name = KEY_NAME_1;

	keys[1].pin = KEY_A3_Pin;
	keys[1].port = KEY_A3_GPIO_Port;
	keys[1].layers[0].key_name = KEY_NAME_2;

	keys[2].pin = KEY_A4_Pin;
	keys[2].port = KEY_A4_GPIO_Port;
	keys[2].layers[0].key_name = KEY_NAME_3;

//	key_init(&keys[ 0], KEY_NAME_1, KEY_A2_GPIO_Port, KEY_A2_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 1], KEY_NAME_2, KEY_A3_GPIO_Port, KEY_A3_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 2], KEY_NAME_3, KEY_A4_GPIO_Port, KEY_A4_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 3], KEY_NAME_4, KEY_A5_GPIO_Port, KEY_A5_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 4], KEY_NAME_5, KEY_A6_GPIO_Port, KEY_A6_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 5], KEY_NAME_Q, KEY_B2_GPIO_Port, KEY_B2_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 6], KEY_NAME_W, KEY_B3_GPIO_Port, KEY_B3_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 7], KEY_NAME_E, KEY_B4_GPIO_Port, KEY_B4_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 8], KEY_NAME_R, KEY_B5_GPIO_Port, KEY_B5_Pin, &keyboard_queue_ptr);
//	key_init(&keys[ 9], KEY_NAME_T, KEY_B6_GPIO_Port, KEY_B6_Pin, &keyboard_queue_ptr);
//	key_init(&keys[10], KEY_NAME_A, KEY_C2_GPIO_Port, KEY_C2_Pin, &keyboard_queue_ptr);
//	key_init(&keys[11], KEY_NAME_S, KEY_C3_GPIO_Port, KEY_C3_Pin, &keyboard_queue_ptr);
//	key_init(&keys[12], KEY_NAME_D, KEY_C4_GPIO_Port, KEY_C4_Pin, &keyboard_queue_ptr);
//	key_init(&keys[13], KEY_NAME_F, KEY_C5_GPIO_Port, KEY_C5_Pin, &keyboard_queue_ptr);
//	key_init(&keys[14], KEY_NAME_G, KEY_C6_GPIO_Port, KEY_C6_Pin, &keyboard_queue_ptr);
//	key_init(&keys[15], KEY_NAME_Z, KEY_D2_GPIO_Port, KEY_D2_Pin, &keyboard_queue_ptr);
//	key_init(&keys[16], KEY_NAME_X, KEY_D3_GPIO_Port, KEY_D3_Pin, &keyboard_queue_ptr);
//	key_init(&keys[17], KEY_NAME_C, KEY_D4_GPIO_Port, KEY_D4_Pin, &keyboard_queue_ptr);
//	key_init(&keys[18], KEY_NAME_V, KEY_D5_GPIO_Port, KEY_D5_Pin, &keyboard_queue_ptr);
//	key_init(&keys[19], KEY_NAME_B, KEY_D6_GPIO_Port, KEY_D6_Pin, &keyboard_queue_ptr);
//	key_init(&keys[20], KEY_NAME_BACKSPACE, KEY_E1_GPIO_Port, KEY_E1_Pin, &keyboard_queue_ptr);
//	key_init(&keys[21], KEY_NAME_SPACE, KEY_E2_GPIO_Port, KEY_E2_Pin, &keyboard_queue_ptr);
//	key_init(&keys[22], KEY_MOD_LSHIFT, KEY_E3_GPIO_Port, KEY_E3_Pin, &keyboard_queue_ptr);
//
//	key_init(&keys[23], KEY_MOD_LALT, KEY_D1_GPIO_Port, KEY_D1_Pin, &keyboard_queue_ptr);


	while (1){

		for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
			key_poll(&keys[i]);
		}


		tx_thread_sleep(10); // TODO: Remove sleep and do proper timing
	}
}

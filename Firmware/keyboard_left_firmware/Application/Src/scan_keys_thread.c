/*
 * scan_keys_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */


#include <key.h>
#include "threads.h"

static key_HandleTypeDef keys[NUMBER_OF_KEYS];

void scan_keys_thread(uint32_t initial_input){

	keys[0].pin = KEY_A1_Pin;
	keys[0].port = KEY_A1_GPIO_Port;
	keys[0].layers[0].key_name = KEY_NAME_ESC;

	keys[1].pin = KEY_A2_Pin;
	keys[1].port = KEY_A2_GPIO_Port;
	keys[1].layers[0].key_name = KEY_NAME_1;

	keys[2].pin = KEY_A3_Pin;
	keys[2].port = KEY_A3_GPIO_Port;
	keys[2].layers[0].key_name = KEY_NAME_2;

	keys[3].pin = KEY_A4_Pin;
	keys[3].port = KEY_A4_GPIO_Port;
	keys[3].layers[0].key_name = KEY_NAME_3;

	keys[4].pin = KEY_A5_Pin;
	keys[4].port = KEY_A5_GPIO_Port;
	keys[4].layers[0].key_name = KEY_NAME_4;

	keys[5].pin = KEY_A6_Pin;
	keys[5].port = KEY_A6_GPIO_Port;
	keys[5].layers[0].key_name = KEY_NAME_5;


	keys[6].pin = KEY_B1_Pin;
	keys[6].port = KEY_B1_GPIO_Port;
	keys[6].layers[0].key_name = KEY_NAME_NONE;

	keys[7].pin = KEY_B2_Pin;
	keys[7].port = KEY_B2_GPIO_Port;
	keys[7].layers[0].key_name = KEY_NAME_Q;

	keys[8].pin = KEY_B3_Pin;
	keys[8].port = KEY_B3_GPIO_Port;
	keys[8].layers[0].key_name = KEY_NAME_W;

	keys[9].pin = KEY_B4_Pin;
	keys[9].port = KEY_B4_GPIO_Port;
	keys[9].layers[0].key_name = KEY_NAME_E;

	keys[10].pin = KEY_B5_Pin;
	keys[10].port = KEY_B5_GPIO_Port;
	keys[10].layers[0].key_name = KEY_NAME_R;

	keys[11].pin = KEY_B6_Pin;
	keys[11].port = KEY_B6_GPIO_Port;
	keys[11].layers[0].key_name = KEY_NAME_T;


	keys[12].pin = KEY_A1_Pin;
	keys[12].port = KEY_A1_GPIO_Port;
	keys[12].layers[0].key_name = KEY_NAME_NONE;

	keys[13].pin = KEY_C2_Pin;
	keys[13].port = KEY_C2_GPIO_Port;
	keys[13].layers[0].key_name = KEY_NAME_A;
	keys[13].layers[0].mod_key = KEY_MOD_LMETA;
	keys[13].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[14].pin = KEY_C3_Pin;
	keys[14].port = KEY_C3_GPIO_Port;
	keys[14].layers[0].key_name = KEY_NAME_S;
	keys[14].layers[0].mod_key = KEY_MOD_LALT;
	keys[14].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[15].pin = KEY_C4_Pin;
	keys[15].port = KEY_C4_GPIO_Port;
	keys[15].layers[0].key_name = KEY_NAME_D;
	keys[15].layers[0].mod_key = KEY_MOD_LSHIFT;
	keys[15].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[16].pin = KEY_C5_Pin;
	keys[16].port = KEY_C5_GPIO_Port;
	keys[16].layers[0].key_name = KEY_NAME_F;
	keys[16].layers[0].mod_key = KEY_MOD_LCTRL;
	keys[16].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[17].pin = KEY_C6_Pin;
	keys[17].port = KEY_C6_GPIO_Port;
	keys[17].layers[0].key_name = KEY_NAME_G;

	keys[18].pin = KEY_D1_Pin;
	keys[18].port = KEY_D1_GPIO_Port;
	keys[18].layers[0].key_name = KEY_NAME_NONE;

	keys[19].pin = KEY_D2_Pin;
	keys[19].port = KEY_D2_GPIO_Port;
	keys[19].layers[0].key_name = KEY_NAME_Z;

	keys[20].pin = KEY_D3_Pin;
	keys[20].port = KEY_D3_GPIO_Port;
	keys[20].layers[0].key_name = KEY_NAME_X;

	keys[21].pin = KEY_D4_Pin;
	keys[21].port = KEY_D4_GPIO_Port;
	keys[21].layers[0].key_name = KEY_NAME_C;

	keys[22].pin = KEY_D5_Pin;
	keys[22].port = KEY_D5_GPIO_Port;
	keys[22].layers[0].key_name = KEY_NAME_V;

	keys[23].pin = KEY_A6_Pin;
	keys[23].port = KEY_A6_GPIO_Port;
	keys[23].layers[0].key_name = KEY_NAME_B;


	keys[24].pin = KEY_E1_Pin;
	keys[24].port = KEY_E1_GPIO_Port;
	keys[24].layers[0].key_name = KEY_NAME_BACKSPACE;
	keys[24].layers[1].key_name = KEY_NAME_BACKSPACE;

	keys[25].pin = KEY_E2_Pin;
	keys[25].port = KEY_E2_GPIO_Port;
	keys[25].layers[0].key_name = KEY_NAME_SPACE;
	keys[25].layers[1].key_name = KEY_NAME_SPACE;

	keys[26].pin = KEY_E3_Pin;
	keys[26].port = KEY_E3_GPIO_Port;
	keys[26].layers[0].mod_key = KEY_MOD_LSHIFT;
	keys[26].layers[1].mod_key = KEY_MOD_LAYER_CHANGE_TOGGLE;
	keys[26].layers[1].layer_change = KEY_LAYER_0;


	for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
		keys[i].key_queue = &keyboard_queue_ptr;
		key_init(&keys[i]);
	}


	while (1){

		for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
			key_poll(&keys[i]);
		}


		tx_thread_sleep(10); // TODO: Remove sleep and do proper timing
	}
}

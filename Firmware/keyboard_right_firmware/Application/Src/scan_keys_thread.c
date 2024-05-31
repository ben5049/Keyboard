/*
 * scan_keys_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */


#include <keys.h>
#include "threads.h"

static key_HandleTypeDef keys[NUMBER_OF_KEYS];
static knob_HandleTypeDef knob;

void scan_keys_thread(uint32_t initial_input){

	keys[0].pin = KEY_A7_Pin;
	keys[0].port = KEY_A7_GPIO_Port;
	keys[0].layers[0].key_name = KEY_NAME_6;

	keys[1].pin = KEY_A8_Pin;
	keys[1].port = KEY_A8_GPIO_Port;
	keys[1].layers[0].key_name = KEY_NAME_7;

	keys[2].pin = KEY_A9_Pin;
	keys[2].port = KEY_A9_GPIO_Port;
	keys[2].layers[0].key_name = KEY_NAME_8;

	keys[3].pin = KEY_A10_Pin;
	keys[3].port = KEY_A10_GPIO_Port;
	keys[3].layers[0].key_name = KEY_NAME_9;

	keys[4].pin = KEY_A11_Pin;
	keys[4].port = KEY_A11_GPIO_Port;
	keys[4].layers[0].key_name = KEY_NAME_0;

	keys[5].pin = KEY_A12_Pin;
	keys[5].port = KEY_A12_GPIO_Port;
	keys[5].layers[0].key_name = KEY_NAME_MINUS;


	keys[6].pin = KEY_B7_Pin;
	keys[6].port = KEY_B7_GPIO_Port;
	keys[6].layers[0].key_name = KEY_NAME_Y;

	keys[7].pin = KEY_B8_Pin;
	keys[7].port = KEY_B8_GPIO_Port;
	keys[7].layers[0].key_name = KEY_NAME_U;

	keys[8].pin = KEY_B9_Pin;
	keys[8].port = KEY_B9_GPIO_Port;
	keys[8].layers[0].key_name = KEY_NAME_I;

	keys[9].pin = KEY_B10_Pin;
	keys[9].port = KEY_B10_GPIO_Port;
	keys[9].layers[0].key_name = KEY_NAME_O;

	keys[10].pin = KEY_B11_Pin;
	keys[10].port = KEY_B11_GPIO_Port;
	keys[10].layers[0].key_name = KEY_NAME_P;

	keys[11].pin = KEY_B12_Pin;
	keys[11].port = KEY_B12_GPIO_Port;
	keys[11].layers[0].key_name = KEY_NAME_EQUAL;


	keys[12].pin = KEY_C7_Pin;
	keys[12].port = KEY_C7_GPIO_Port;
	keys[12].layers[0].key_name = KEY_NAME_H;

	keys[13].pin = KEY_C8_Pin;
	keys[13].port = KEY_C8_GPIO_Port;
	keys[13].layers[0].key_name = KEY_NAME_J;
	keys[13].layers[0].mod_key = KEY_MOD_RCTRL;
	keys[13].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[14].pin = KEY_C9_Pin;
	keys[14].port = KEY_C9_GPIO_Port;
	keys[14].layers[0].key_name = KEY_NAME_K;
	keys[14].layers[0].mod_key = KEY_MOD_RSHIFT;
	keys[14].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[15].pin = KEY_C10_Pin;
	keys[15].port = KEY_C10_GPIO_Port;
	keys[15].layers[0].key_name = KEY_NAME_L;
	keys[15].layers[0].mod_key = KEY_MOD_RALT;
	keys[15].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[16].pin = KEY_C11_Pin;
	keys[16].port = KEY_C11_GPIO_Port;
	keys[16].layers[0].key_name = KEY_NAME_SEMICOLON;
	keys[16].layers[0].mod_key = KEY_MOD_RMETA;
	keys[16].layers[0].mod_delay = HOME_ROW_MOD_DELAY;

	keys[17].pin = KEY_C12_Pin;
	keys[17].port = KEY_C12_GPIO_Port;
	keys[17].layers[0].key_name = KEY_NAME_LEFTBRACE;


	keys[18].pin = KEY_D7_Pin;
	keys[18].port = KEY_D7_GPIO_Port;
	keys[18].layers[0].key_name = KEY_NAME_N;

	keys[19].pin = KEY_D8_Pin;
	keys[19].port = KEY_D8_GPIO_Port;
	keys[19].layers[0].key_name = KEY_NAME_M;

	keys[20].pin = KEY_D9_Pin;
	keys[20].port = KEY_D9_GPIO_Port;
	keys[20].layers[0].key_name = KEY_NAME_COMMA;

	keys[21].pin = KEY_D10_Pin;
	keys[21].port = KEY_D10_GPIO_Port;
	keys[21].layers[0].key_name = KEY_NAME_DOT;

	keys[22].pin = KEY_D11_Pin;
	keys[22].port = KEY_D11_GPIO_Port;
	keys[22].layers[0].key_name = KEY_NAME_SLASH;

	keys[23].pin = KEY_D12_Pin;
	keys[23].port = KEY_D12_GPIO_Port;
	keys[23].layers[0].key_name = KEY_NAME_RIGHTBRACE;


	keys[24].pin = KEY_E4_Pin;
	keys[24].port = KEY_E4_GPIO_Port;
	keys[24].layers[0].mod_key = KEY_MOD_RCTRL;
	keys[24].layers[1].mod_key = KEY_MOD_RCTRL;

	keys[25].pin = KEY_E5_Pin;
	keys[25].port = KEY_E5_GPIO_Port;
	keys[25].layers[0].key_name = KEY_NAME_ENTER;
	keys[25].layers[1].key_name = KEY_NAME_ENTER;

	keys[26].pin = KEY_E6_Pin;
	keys[26].port = KEY_E6_GPIO_Port;
	keys[26].layers[0].mod_key = KEY_MOD_LAYER_CHANGE_TOGGLE;
	keys[26].layers[0].layer_change = KEY_LAYER_1;
	keys[26].layers[1].mod_key = KEY_MOD_LAYER_CHANGE_TOGGLE;
	keys[26].layers[1].layer_change = KEY_LAYER_0;


	for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
		keys[i].event_queue = &key_event_queue_ptr;
		key_init(&keys[i]);
	}

	knob.layers[0].up_key_name = KEY_NAME_DOWN;
	knob.layers[0].down_key_name = KEY_NAME_UP;
	knob_init(&knob, &ENCODER_HANDLE, &key_event_queue_ptr);

	while (1){

		for (uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
			key_poll(&keys[i]);
		}

		knob_poll(&knob);

		tx_thread_sleep(10); // TODO: Remove sleep and do proper timing
	}
}

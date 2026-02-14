/*
 * keys.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include <keys.h>
#include <system_state.h>

/* Private function prototypes */
keyboardHID_StatusTypeDef keyboardHID_recieve_event(keyboardHID_HandleTypeDef *keyboard, uint32_t timeout);
keyboardHID_StatusTypeDef keyboardHID_recieve_event_no_wait(keyboardHID_HandleTypeDef *keyboard);
keyboardHID_StatusTypeDef keyboardHID_send_report(keyboardHID_HandleTypeDef *keyboard, bool send_taps);
keyboardHID_StatusTypeDef keyboardHID_add_key(keyboardHID_HandleTypeDef *keyboard, keyName_TypeDef key, bool tap);
keyboardHID_StatusTypeDef keyboardHID_remove_key(keyboardHID_HandleTypeDef *keyboard, keyName_TypeDef key);
keyboardHID_StatusTypeDef keyboardHID_untap_keys(keyboardHID_HandleTypeDef *keyboard);

void key_init(key_HandleTypeDef *key){

	key->current_state = KEY_RELEASED;
	key->previous_state = KEY_RELEASED;
	key->current_layer = 0;
	key->press_timestamp = tx_time_get();
	key->holding_mod_key = false;
}

void key_poll(key_HandleTypeDef *key){

	/* Get the current layer. No wait version used to reduce system mutex taking overhead. */
	system_layer_get_no_wait(&key->current_layer);

	/* Get the current pin state */
	if (HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET){
		key->current_state = KEY_PRESSED;
	}
	else {
		key->current_state = KEY_RELEASED;
	}

	/* Determine action to be taken */

	/* Key pressed */
	if (key->current_state == KEY_PRESSED && key->previous_state == KEY_RELEASED){
		key->press_timestamp = tx_time_get();

		/* Mod key pressed, immediate action */
		if (key->layers[key->current_layer].mod_key){

			/* Execute mod key press */
			key->holding_mod_key = true;

			switch (key->layers[key->current_layer].mod_key) {

			case KEY_MOD_LAYER_CHANGE_MOMENTARY:
				system_layer_change(key->layers[key->current_layer].layer_change, true);
				break;

			case KEY_MOD_LAYER_CHANGE_TOGGLE:
				system_layer_change(key->layers[key->current_layer].layer_change, false);
				/* TODO: This needs to invalidate the associated key hold and release */
				break;

			case KEY_MOD_LMETA:
				if (key->layers[key->current_layer].mod_delay == 0){
					system_modifier_set(key->layers[key->current_layer].mod_key);
					keyEvent_TypeDef event;
					event.key = KEY_NAME_NONE;
					event.state = KEY_TAP;
					tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
				}
				else {
					key->holding_mod_key = false;
				}
				break;

			case KEY_MOD_RMETA:
				if (key->layers[key->current_layer].mod_delay == 0){
					system_modifier_set(key->layers[key->current_layer].mod_key);
					keyEvent_TypeDef event;
					event.key = KEY_NAME_NONE;
					event.state = KEY_TAP;
					tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
				}
				else {
					key->holding_mod_key = false;
				}
				break;

			case KEY_MOD_LALT:
				if (key->layers[key->current_layer].mod_delay == 0){
					system_modifier_set(key->layers[key->current_layer].mod_key);
					keyEvent_TypeDef event;
					event.key = KEY_NAME_NONE;
					event.state = KEY_TAP;
					tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
				}
				else {
					key->holding_mod_key = false;
				}
				break;

			default:
				system_modifier_set(key->layers[key->current_layer].mod_key);
				keyEvent_TypeDef event;
				event.key = KEY_NAME_NONE;
				event.state = KEY_TAP;
				tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
				break;
			}
		}

		/* Normal key pressed */
		else {
			key->holding_mod_key = false;
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;
			event.state = KEY_PRESSED;
			tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
		}
	}


	/* Key released */
	else if (key->current_state == KEY_RELEASED && key->previous_state == KEY_PRESSED){

		/* Mod key released */
		if (key->holding_mod_key || key->layers[key->current_layer].mod_delay > 0){

			key->holding_mod_key = false;

			/* Normal mod key (not home row mod) */
			if (key->layers[key->current_layer].mod_delay == 0){

				if (key->layers[key->current_layer].mod_key == KEY_MOD_LAYER_CHANGE_MOMENTARY) {
					system_layer_revert();
				}

				else {
					system_modifier_reset(key->layers[key->current_layer].mod_key);
					keyEvent_TypeDef event;
					event.key = KEY_NAME_NONE;
					event.state = KEY_TAP;
					tx_queue_send(key->event_queue, &event, TX_WAIT_FOREVER);
				}
			}

			/* Home row mod less than threshold time */
			else if ((((tx_time_get() - key->press_timestamp) * 1000) / TX_TIMER_TICKS_PER_SECOND) < key->layers[key->current_layer].mod_delay){
				system_modifier_reset(key->layers[key->current_layer].mod_key);
				keyEvent_TypeDef event;
				event.key = key->layers[key->current_layer].key_name;
				event.state = KEY_TAP;
				tx_queue_send(key->event_queue, &event, TX_WAIT_FOREVER);
			}

			/* Home row mod more than threshold time */
			else {
				system_modifier_reset(key->layers[key->current_layer].mod_key);
				keyEvent_TypeDef event;
				event.key = KEY_NAME_NONE;
				event.state = KEY_TAP;
				tx_queue_send(key->event_queue, &event, TX_WAIT_FOREVER);
			}
		}

		/* Normal key released */
		else {
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;
			event.state = KEY_RELEASED;
			tx_queue_send(key->event_queue, &event, TX_WAIT_FOREVER);
		}
	}

	/* Key held */
	else if (key->current_state == KEY_PRESSED && key->previous_state == KEY_PRESSED){

		/* If there is a timed mod key that hasn't been fired then wait and fire when delay expires. (Usually meta keys) */
		if ((!key->holding_mod_key) && (key->layers[key->current_layer].mod_delay > 0) && ((((tx_time_get() - key->press_timestamp) * 1000) / TX_TIMER_TICKS_PER_SECOND) > key->layers[key->current_layer].mod_delay)){
			key->holding_mod_key = true;
			system_modifier_set(key->layers[key->current_layer].mod_key);
			keyEvent_TypeDef event;
			event.key = KEY_NAME_NONE;
			event.state = KEY_TAP;
			tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
		}
	}

	/* Update variables for next time */
	key->previous_state = key->current_state;
}





void knob_init(knob_HandleTypeDef *knob, TIM_HandleTypeDef *htim, TX_QUEUE *event_queue){
	knob->current_layer = 0;
	knob->event_queue = event_queue;

	knob->htim = htim;
	HAL_TIM_Encoder_Start(knob->htim, TIM_CHANNEL_ALL);

	knob->current_count = knob->htim->Instance->CNT;
	knob->previous_count = knob->current_count;
}

void knob_poll(knob_HandleTypeDef *knob){

	/* Get the current layer. No wait version used to reduce system mutex taking overhead. */
	system_layer_get_no_wait(&knob->current_layer);

	/* Load the timer's count into current_count. Note CNT has to be cast from a uint32_t to an int16_t. This is fine as long as the timer's period is 65535. */
	__atomic_load((int16_t*) &knob->htim->Instance->CNT, &knob->current_count, __ATOMIC_RELAXED);

	/* Only register an input every ENCODER_PULSES_PER_INDENT pulses. */
	if (((knob->current_count - knob->previous_count) % ENCODER_PULSES_PER_INDENT) == 0){

		/* If the counter incremented send the current layer's up key */
		if (knob->current_count > knob->previous_count){

			keyEvent_TypeDef event;
			event.key = knob->layers[knob->current_layer].up_key_name;
			event.state = KEY_TAP;

			for (uint8_t i = 0; i < (knob->current_count - knob->previous_count) / ENCODER_PULSES_PER_INDENT; i++){
				tx_queue_send(knob->event_queue, &event, TX_NO_WAIT);
			}
		}

		/* If the counter decremented send the current layer's down key */
		else if (knob->current_count < knob->previous_count) {

			keyEvent_TypeDef event;
			event.key = knob->layers[knob->current_layer].down_key_name;
			event.state = KEY_TAP;

			for (uint8_t i = 0; i < (knob->previous_count - knob->current_count) / ENCODER_PULSES_PER_INDENT; i++){
				tx_queue_send(knob->event_queue, &event, TX_NO_WAIT);
			}
		}

		knob->previous_count = knob->current_count;
	}
}





keyboardHID_StatusTypeDef keyboardHID_init(keyboardHID_HandleTypeDef *keyboard, TX_QUEUE *event_queue, UX_SLAVE_CLASS_HID *hid_class){

	keyboard->event_queue = event_queue;
	keyboard->hid_class = hid_class;
	keyboard->key_event.key = KEY_NAME_NONE;
	keyboard->key_event.state = KEY_RELEASED;
	keyboard->keys_pressed = NULL;
	keyboard->send_immediately = false;

	/* Reset the HID event structure */
	ux_utility_memory_set(&keyboard->hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

	keyboard->initialised = true;

	return KEYBOARD_HID_OK;
}

keyboardHID_StatusTypeDef keyboardHID_process(keyboardHID_HandleTypeDef *keyboard){

	/* Properly implement status */
	keyboardHID_StatusTypeDef status = KEYBOARD_HID_OK;

	/* If the next report doesn't need to be send immediately then get a new key event */
	if (!keyboard->send_immediately){
		status = keyboardHID_recieve_event(keyboard, 100);
		if (status != KEYBOARD_HID_OK){
			return status;
		}

		/* Process the event */
		bool queue_empty = false;
		do {
			switch (keyboard->key_event.state) {

			case KEY_PRESSED:
				keyboardHID_add_key(keyboard, keyboard->key_event.key, false);
				break;

			case KEY_RELEASED:
				keyboardHID_remove_key(keyboard, keyboard->key_event.key);
				break;

			case KEY_TAP:
				keyboardHID_add_key(keyboard, keyboard->key_event.key, true);
				break;

			default:
				break;
			}

			/* Try to get another event */
			if (keyboardHID_recieve_event_no_wait(keyboard) == KEYBOARD_HID_QUEUE_EMPTY){
				queue_empty = true;
			}

		} while (!queue_empty);

	}

	/* Send the report */
	keyboardHID_send_report(keyboard, true);

	/* Untap keys and send another report if necessary */
	if (keyboardHID_untap_keys(keyboard) == KEYBOARD_HID_FOUND_TAPPED){
		keyboardHID_send_report(keyboard, false);
	}

	return status;
}


keyboardHID_StatusTypeDef keyboardHID_recieve_event(keyboardHID_HandleTypeDef *keyboard, uint32_t timeout){
	if (tx_queue_receive(keyboard->event_queue, &keyboard->key_event, timeout) == TX_SUCCESS){
		return KEYBOARD_HID_OK;
	}
	return KEYBOARD_HID_ERROR;
}

keyboardHID_StatusTypeDef keyboardHID_recieve_event_no_wait(keyboardHID_HandleTypeDef *keyboard){
	if (tx_queue_receive(keyboard->event_queue, &keyboard->key_event, TX_NO_WAIT) == TX_SUCCESS){
		return KEYBOARD_HID_OK;
	}
	return KEYBOARD_HID_QUEUE_EMPTY;
}

keyboardHID_StatusTypeDef keyboardHID_send_report(keyboardHID_HandleTypeDef *keyboard, bool send_taps){

	/* Get modifier byte */
	system_modifiers_get(&keyboard->hid_event.ux_device_class_hid_event_buffer[0]);

	/* This byte is reserved */
	keyboard->hid_event.ux_device_class_hid_event_buffer[1] = 0;

	/* Clear the buffer */
	for (uint8_t i = 2; i < 8; i++){
		keyboard->hid_event.ux_device_class_hid_event_buffer[i] = 0;
	}

	/* Go through the linked list of keys and copy them to the message buffer */
	keyboard->num_keys_pressed = 0;
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while ((*current != NULL) && (keyboard->num_keys_pressed <= MAX_CONCURRENT_KEYS)) {

		/* Only copy the key to the buffer if it isn't tapped, or if it is tapped and we want to send tapped keys */
		if (!(*current)->tap || (*current)->tap == send_taps){
			keyboard->hid_event.ux_device_class_hid_event_buffer[keyboard->num_keys_pressed + 2] = (*current)->key;
			keyboard->num_keys_pressed++;
		}
		current = &((*current)->next);
	}

	/* Set the length of the message */
	//	keyboard->hid_event.ux_device_class_hid_event_length = keyboard->num_keys_pressed + 2; /*TODO: Figure out device descriptors to send a longer message */
	keyboard->hid_event.ux_device_class_hid_event_length = 8;

	/* Send keyboard event */
	ux_device_class_hid_event_set(keyboard->hid_class, &keyboard->hid_event);

	return KEYBOARD_HID_OK;
}

keyboardHID_StatusTypeDef keyboardHID_add_key(keyboardHID_HandleTypeDef *keyboard, keyName_TypeDef key, bool tap){

	/* NONE key is always tapped and never held */
	if (key == KEY_NAME_NONE){
		tap = true;
	}

	/* Check the key isn't already pressed */
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while (*current != NULL) {
		if ((*current)->key == key){

			/* If the key is already tapped and new event is another tap then increment the taps */
			if ((*current)->tap && tap){
				(*current)->taps++;
			}

			/* Otherwise update the key and return */
			else {
				(*current)->tap = tap;
				(*current)->taps = 0;
			}
			return KEYBOARD_HID_ALREADY_PRESSED;
		}
		current = &((*current)->next);
	}

	/* Check there is space in the linked list */
	if (keyboard->num_keys_pressed >= MAX_CONCURRENT_KEYS){
		return KEYBOARD_HID_LIST_FULL;
	}

	/* Allocate the memory for the new node */
	keyNode_TypeDef* new_node = (keyNode_TypeDef*)malloc(sizeof(keyNode_TypeDef));

	/* Populate the new node */
	new_node->key = key;
	new_node->tap = tap;
	new_node->taps = 0;

	/* Assign the old head to this node's "next" pointer, and make this node the new head */
	new_node->next = keyboard->keys_pressed;
	keyboard->keys_pressed = new_node;

	keyboard->num_keys_pressed++;

	return KEYBOARD_HID_OK;
}

keyboardHID_StatusTypeDef keyboardHID_remove_key(keyboardHID_HandleTypeDef *keyboard, keyName_TypeDef key){

	/* Go through linked list */
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while (*current != NULL) {

		/* If the current node contains the key then copy the current node to a temporary node, set the current node to be the one after the temporary node, and free the temporary node */
		if ((*current)->key == key) {
			keyNode_TypeDef* temp = *current;
			*current = (*current)->next;
			free(temp);
			keyboard->num_keys_pressed--;
			return KEYBOARD_HID_OK;
		}
		current = &((*current)->next);
	}

	return KEYBOARD_HID_KEY_NOT_FOUND;
}

keyboardHID_StatusTypeDef keyboardHID_untap_keys(keyboardHID_HandleTypeDef *keyboard){

	keyboardHID_StatusTypeDef status = KEYBOARD_HID_NONE_TAPPED;

	keyboard->send_immediately = false;

	/* Go through linked list */
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while (*current != NULL) {

		/* If the current node is tapped and has no taps left then remove it: copy the current node to a temporary node, set the current node to be the one after the temporary node, and free the temporary node */
		if ((*current)->tap && (*current)->taps == 0) {
			keyNode_TypeDef* temp = *current;
			*current = (*current)->next;
			free(temp);
			keyboard->num_keys_pressed--;
			status = KEYBOARD_HID_FOUND_TAPPED;
		}

		/* If the current node is tapped but has taps left then decrement the number of taps and set send_immediately to true to send the next tap */
		else if ((*current)->tap){
			(*current)->taps--;
			current = &((*current)->next);
			status = KEYBOARD_HID_FOUND_TAPPED;
			keyboard->send_immediately = true;
		}
		else {
			current = &((*current)->next);
		}
	}

	return status;
}


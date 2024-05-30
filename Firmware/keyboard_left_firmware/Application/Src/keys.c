/*
 * keys.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include <keys.h>
#include <system_state.h>

/* Private function prototypes */
keyboardHID_StatusTypeDef keyboardHID_recieve_event(keyboardHID_HandleTypeDef *keyboard);
keyboardHID_StatusTypeDef keyboardHID_recieve_event_no_wait(keyboardHID_HandleTypeDef *keyboard);
keyboardHID_StatusTypeDef keyboardHID_send_report(keyboardHID_HandleTypeDef *keyboard);
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

		/* Mod key pressed, immediate action */
		if (key->layers[key->current_layer].mod_key && key->layers[key->current_layer].mod_delay == 0){

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

			default:
				system_modifier_set(key->layers[key->current_layer].mod_key);
				keyEvent_TypeDef event;
				event.key = KEY_NAME_NONE;
				event.state = KEY_TAP;
				tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
				break;
			}
		}


		/* Normal/mod key pressed, delayed action (probably a home row mod) */
		else if (key->layers[key->current_layer].mod_key && key->layers[key->current_layer].mod_delay > 0){
			key->holding_mod_key = false;
			key->press_timestamp = tx_time_get();
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
		if (key->holding_mod_key){

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

		/* Normal/mod key released before mod key triggered. Send press and release messages in quick succession */
		else if (key->layers[key->current_layer].mod_key){
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;
			event.state = KEY_TAP;
			tx_queue_send(key->event_queue, &event, TX_NO_WAIT);
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

		/* Delayed mod key reaches trigger time */
		if ((key->layers[key->current_layer].mod_key) && ((((tx_time_get() - key->press_timestamp) * 1000) / TX_TIMER_TICKS_PER_SECOND) > key->layers[key->current_layer].mod_delay) && (key->holding_mod_key == false)){
			key->holding_mod_key = true;
			system_modifier_set(key->layers[key->current_layer].mod_key);

			keyEvent_TypeDef event;
			event.key = KEY_NAME_NONE;
			event.state = KEY_PRESSED;
			tx_queue_send(key->event_queue, &event, TX_WAIT_FOREVER);
		}
	}

	/* Update variables for next time */
	key->previous_state = key->current_state;
}



keyboardHID_StatusTypeDef keyboardHID_init(keyboardHID_HandleTypeDef *keyboard, TX_QUEUE *event_queue, UX_SLAVE_CLASS_HID *hid_class){

	keyboard->event_queue = event_queue;
	keyboard->hid_class = hid_class;
	keyboard->key_event.key = KEY_NAME_NONE;
	keyboard->key_event.state = KEY_RELEASED;
	keyboard->keys_pressed = NULL;

	/* Reset the HID event structure */
	ux_utility_memory_set(&keyboard->hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

	keyboard->initialised = true;

	return KEYBOARD_HID_OK;
}

keyboardHID_StatusTypeDef keyboardHID_process(keyboardHID_HandleTypeDef *keyboard){

	/* Get a key event, this is blocking so will wait forever */
	keyboardHID_recieve_event(keyboard);

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

	/* Send the report */
	keyboardHID_send_report(keyboard);

	/* Untap keys and send another report if necessary */
	if (keyboardHID_untap_keys(keyboard) == KEYBOARD_HID_FOUND_TAPPED){
		keyboardHID_send_report(keyboard);
	}

	return KEYBOARD_HID_OK;
}


keyboardHID_StatusTypeDef keyboardHID_recieve_event(keyboardHID_HandleTypeDef *keyboard){
	if (tx_queue_receive(keyboard->event_queue, &keyboard->key_event, TX_WAIT_FOREVER) == TX_SUCCESS){
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

keyboardHID_StatusTypeDef keyboardHID_send_report(keyboardHID_HandleTypeDef *keyboard){

	/* Get modifier byte */
	system_modifiers_get(&keyboard->hid_event.ux_device_class_hid_event_buffer[0]);

	/* This byte is reserved */
	keyboard->hid_event.ux_device_class_hid_event_buffer[1] = 0;

	/* Go through the linked list of keys and copy them to the message buffer */
	for (uint8_t i = 2; i < 8; i++){
		keyboard->hid_event.ux_device_class_hid_event_buffer[i] = 0;
	}

	keyboard->num_keys_pressed = 0;
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while ((*current != NULL) && (keyboard->num_keys_pressed <= MAX_CONCURRENT_KEYS)) {
		keyboard->hid_event.ux_device_class_hid_event_buffer[keyboard->num_keys_pressed + 2] = (*current)->key;
		keyboard->num_keys_pressed++;
		current = &((*current)->next);
	}

	//	for (uint8_t i = keyboard->num_keys_pressed + 2; i < MAX_CONCURRENT_KEYS + 2; i++){

	//	}

	/* Set the length of the message */
	keyboard->hid_event.ux_device_class_hid_event_length = keyboard->num_keys_pressed + 2;
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

			/* If key is already pressed then update it */
			(*current)->tap = tap;
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

	/* Go through linked list */
	keyNode_TypeDef** current = &keyboard->keys_pressed;
	while (*current != NULL) {

		/* If the current node is tapped (or none) then copy the current node to a temporary node, set the current node to be the one after the temporary node, and free the temporary node */
		if (((*current)->tap) || ((*current)->key == KEY_NAME_NONE)) {
			keyNode_TypeDef* temp = *current;
			*current = (*current)->next;
			free(temp);
			keyboard->num_keys_pressed--;
			status = KEYBOARD_HID_FOUND_TAPPED;
		}
		else {
			current = &((*current)->next);
		}
	}

	return status;
}


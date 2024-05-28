/*
 * keys.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include "keys.h"
#include "system.h"

void key_init(key_HandleTypeDef *key){

	key->current_state = HAL_GPIO_ReadPin(key->port, key->pin);
	key->previous_state = key->current_state;
	key->current_layer = 0;
	key->press_timestamp = tx_time_get();
	key->holding_mod_key = false;
}

void key_poll(key_HandleTypeDef *key){

	/* Get the current layer. No wait version used to reduce system mutex taking overhead. */
	key->current_layer = system_layer_get_no_wait();

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
		if (key->layers[key->current_layer].mod_enable && key->layers[key->current_layer].mod_delay == 0){

			/* Execute mod key press (layer changes are done on release) */
			key->holding_mod_key = true;
			if (key->layers[key->current_layer].mod_key != KEY_MOD_LAYER_CHANGE){
				system_modifier_set(key->layers[key->current_layer].mod_key);

				keyEvent_TypeDef event;
				event.key = KEY_NAME_NONE;
				event.state = KEY_PRESSED;
				tx_queue_send(key->key_queue, &event, TX_NO_WAIT);
			}
		}

		/* Normal/mod key pressed, delayed action (probably a home row mod) */
		else if (key->layers[key->current_layer].mod_enable && key->layers[key->current_layer].mod_delay > 0){
			key->holding_mod_key = false;
			key->press_timestamp = tx_time_get();
		}

		/* Normal key pressed */
		else {
			key->holding_mod_key = false;
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;
			event.state = KEY_PRESSED;
			tx_queue_send(key->key_queue, &event, TX_NO_WAIT);
		}
	}


	/* Key released */
	else if (key->current_state == KEY_RELEASED && key->previous_state == KEY_PRESSED){

		/* Mod key released */
		if (key->holding_mod_key){

			/* Layer change */
			if (key->layers[key->current_layer].mod_key == KEY_MOD_LAYER_CHANGE){
				system_layer_change(key->layers[key->current_layer].layer_change);
			}

			/* Release mod key */
			else {
				system_modifier_reset(key->layers[key->current_layer].mod_key);
				keyEvent_TypeDef event;
				event.key = KEY_NAME_NONE;
				event.state = KEY_RELEASED;
				tx_queue_send(key->key_queue, &event, TX_WAIT_FOREVER);
			}
		}

		/* Normal/mod key released before mod key triggered. Send press and release messages in quick succession */
		else if (key->layers[key->current_layer].mod_enable){
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;

			event.state = KEY_PRESSED;
			tx_queue_send(key->key_queue, &event, TX_NO_WAIT);

			event.state = KEY_RELEASED;
			tx_queue_send(key->key_queue, &event, TX_WAIT_FOREVER);
		}

		/* Normal key released */
		else {
			keyEvent_TypeDef event;
			event.key = key->layers[key->current_layer].key_name;
			event.state = KEY_RELEASED;
			tx_queue_send(key->key_queue, &event, TX_WAIT_FOREVER);
		}
	}


	/* Key held */
	else if (key->current_state == KEY_PRESSED && key->previous_state == KEY_PRESSED){

		/* Delayed mod key reaches trigger time */
		if ((key->layers[key->current_layer].mod_enable) && ((tx_time_get() - key->press_timestamp) > key->layers[key->current_layer].mod_delay) && (key->holding_mod_key == false)){
			key->holding_mod_key = true;
			system_modifier_set(key->layers[key->current_layer].mod_key);

			keyEvent_TypeDef event;
			event.key = KEY_NAME_NONE;
			event.state = KEY_PRESSED;
			tx_queue_send(key->key_queue, &event, TX_WAIT_FOREVER);
		}
	}

	/* Update variables for next time */
	key->previous_state = key->current_state;
}

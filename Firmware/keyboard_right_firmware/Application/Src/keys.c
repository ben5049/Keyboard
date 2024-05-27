/*
 * keys.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include "keys.h"

void key_init(key_HandleTypeDef *key, keyName_TypeDef key_name, GPIO_TypeDef *port, uint16_t pin, TX_QUEUE *queue){

	key->key_name = key_name;
	key->port = port;
	key->pin = pin;
	key->queue = queue;

	key->current_state = HAL_GPIO_ReadPin(key->port, key->pin);
	key->previous_state = key->current_state;
}

void key_update(key_HandleTypeDef *key){

	key->current_state = HAL_GPIO_ReadPin(key->port, key->pin);

	/* Key pressed */
	if (key->current_state == GPIO_PIN_RESET && key->previous_state == GPIO_PIN_SET){

		key->event.key = key->key_name;
		key->event.state = KEY_PRESSED;

		tx_queue_send(key->queue, &key->event, TX_NO_WAIT);
	}

	/* Key released */
	else if (key->current_state == GPIO_PIN_SET && key->previous_state == GPIO_PIN_RESET){

		key->event.key = key->key_name;
		key->event.state = KEY_RELEASED;

		tx_queue_send(key->queue, &key->event, TX_NO_WAIT);
	}

	key->previous_state = key->current_state;
}

/*
 * system.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include "system_state.h"
#include "tx_api.h"
#include "usbpd.h"


/* PFP */
systemState_StatusTypeDef system_state_lock(uint32_t timeout);
systemState_StatusTypeDef system_state_unlock(void);


static struct {
	TX_MUTEX *mutex; /* Mutex to protect system */

	volatile uint8_t momentary_layer; /* Current keyboard layer. Between 0 and NUMBER_OF_LAYERS-1 */
	volatile uint8_t actual_layer; /* Current keyboard layer. Between 0 and NUMBER_OF_LAYERS-1 */

	volatile uint8_t hid_modifiers; /* HID Modifiers, e.g. CTRL, SHIFT, ALT etc */
} system_state;

static TX_MUTEX system_state_mutex;


systemState_StatusTypeDef system_init(void){

	system_state.actual_layer = KEY_LAYER_0;
	system_state.momentary_layer = KEY_LAYER_0;
	system_state.hid_modifiers = KEY_MOD_NONE;

	system_state.mutex = &system_state_mutex;
	if (tx_mutex_create(system_state.mutex, "System state mutex", TX_INHERIT) != TX_SUCCESS) {
		return SYSTEM_MUTEX_ERROR;
	}

	return SYSTEM_OK;
}



systemState_StatusTypeDef system_modifier_set(keyMod_TypeDef modifier){
	if (modifier > 0x80){
		return SYSTEM_ERROR;
	}

	system_state_lock(TX_WAIT_FOREVER);

	system_state.hid_modifiers |= modifier;

	system_state_unlock();

	return SYSTEM_OK;
}

systemState_StatusTypeDef system_modifier_reset(keyMod_TypeDef modifier){
	if (modifier > 0x80){
		return SYSTEM_ERROR;
	}

	system_state_lock(TX_WAIT_FOREVER);

	system_state.hid_modifiers &= ~modifier;

	system_state_unlock();

	return SYSTEM_OK;
}

systemState_StatusTypeDef system_modifiers_get(uint8_t *modifiers){

	system_state_lock(TX_WAIT_FOREVER);

	*modifiers = system_state.hid_modifiers;

	system_state_unlock();

	return SYSTEM_OK;
}



systemState_StatusTypeDef system_layer_change(keyLayerChange_TypeDef layer_change, bool momentary){

	systemState_StatusTypeDef status = SYSTEM_OK;

	status = system_state_lock(TX_WAIT_FOREVER);
	if (status != SYSTEM_OK) {
		return status;
	}


	switch (layer_change) {

	case KEY_LAYER_NEXT:
		if (++system_state.momentary_layer >= NUMBER_OF_LAYERS){
			system_state.momentary_layer = 0;
		}
		break;

	case KEY_LAYER_PREV:
		if (--system_state.momentary_layer >= NUMBER_OF_LAYERS){
			system_state.momentary_layer = NUMBER_OF_LAYERS - 1;
		}
		break;

	default:
		system_state.momentary_layer = layer_change;
		break;
	}


	if (!momentary) {
		system_state.actual_layer = system_state.momentary_layer;
	}


	HAL_GPIO_WritePin(STAT1_LED_GPIO_Port, STAT1_LED_Pin, system_state.actual_layer); // TODO: DELETE LINE

	status = system_state_unlock();

	return status;
}

systemState_StatusTypeDef system_layer_revert(void){



	return SYSTEM_OK;
}

systemState_StatusTypeDef system_layer_get(uint8_t *layer){

	*layer = __atomic_load_n(&system_state.actual_layer, __ATOMIC_RELAXED);

	return SYSTEM_OK;
}

systemState_StatusTypeDef system_layer_get_no_wait(uint8_t *layer){

	*layer = __atomic_load_n(&system_state.actual_layer, __ATOMIC_RELAXED);

	return SYSTEM_OK;
}







systemState_StatusTypeDef system_state_lock(uint32_t timeout){
	switch (tx_mutex_get(system_state.mutex, timeout)) {

	case TX_MUTEX_ERROR:
		return SYSTEM_MUTEX_ERROR;

	case TX_WAIT_ERROR:
		return SYSTEM_TIMEOUT;

	case TX_SUCCESS:
		return SYSTEM_OK;

	default:
		return SYSTEM_ERROR;
	}
}

systemState_StatusTypeDef system_state_unlock(){
	switch (tx_mutex_put(system_state.mutex)) {

	case TX_MUTEX_ERROR:
		return SYSTEM_MUTEX_ERROR;

	case TX_SUCCESS:
		return SYSTEM_OK;

	default:
		return SYSTEM_ERROR;
	}
}

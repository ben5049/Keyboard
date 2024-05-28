/*
 * system.h
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stdint.h"
#include "tx_api.h"
#include "keys.h"

typedef struct {
	TX_MUTEX *mutex; /* Mutex to protect system */

	volatile uint8_t layer; /* Current keyboard layer. Between 0 and NUMBER_OF_LAYERS-1 */

	uint8_t hid_modifiers; /* HID Modifiers, e.g. CTRL, SHIFT, ALT etc */
	TX_SEMAPHORE *modifier_change_semaphore; /* Binary semaphore that increments when a change to hid_modifiers occurs TODO: use event flags? */
} system_TypeDef;

/*
	@ACTION: USER_CODE_INSERT
    @FILE: "Core/Src/app_threadx.c"
    @SECTION: "Includes"
    @INSERT: "#include "system.h""
	@SECTION: "App_ThreadX_Init"
    @INSERT: "system_init();"
 */
void system_init(void);

void system_modifier_set(keyMod_TypeDef modifier);
void system_modifier_reset(keyMod_TypeDef modifier);
uint8_t system_modifiers_get(void);

void system_layer_change(keyLayerChange_TypeDef layer_change);
uint8_t system_layer_get(void);
uint8_t system_layer_get_no_wait(void);

#endif /* INC_SYSTEM_H_ */

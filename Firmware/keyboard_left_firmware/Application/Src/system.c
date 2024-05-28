/*
 * system.c
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#include "system.h"

static system_TypeDef keyboard_system;

void system_init(void){

}

void system_modifier_set(keyMod_TypeDef modifier){

}
void system_modifier_reset(keyMod_TypeDef modifier){

}
uint8_t system_modifiers_get(void){
	return 0;
}

void system_layer_change(keyLayerChange_TypeDef layer_change){

}
uint8_t system_layer_get(void){
	return 0;
}
uint8_t system_layer_get_no_wait(void){
	return 0;
}

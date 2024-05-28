/*
 * system.h
 *
 *  Created on: May 27, 2024
 *      Author: bens1
 */

#ifndef INC_SYSTEM_STATE_H_
#define INC_SYSTEM_STATE_H_

#include <key.h>
#include "stdint.h"


typedef enum
{
  SYSTEM_OK       	 = 0x00U,
  SYSTEM_ERROR    	 = 0x01U,
  SYSTEM_BUSY     	 = 0x02U,
  SYSTEM_TIMEOUT	 = 0x03U,
  SYSTEM_MUTEX_ERROR = 0x04U
} systemState_StatusTypeDef;



/*
	@ACTION: USER_CODE_INSERT
    @FILE: "Core/Src/app_threadx.c"
    @SECTION: "Includes"
    @INSERT: "#include "system_state.h""
	@SECTION: "App_ThreadX_Init"
    @INSERT: "system_init();"
 */
systemState_StatusTypeDef system_init(void);

systemState_StatusTypeDef system_modifier_set(keyMod_TypeDef modifier);
systemState_StatusTypeDef system_modifier_reset(keyMod_TypeDef modifier);
systemState_StatusTypeDef system_modifiers_get(uint8_t *modifiers);

systemState_StatusTypeDef system_layer_change(keyLayerChange_TypeDef layer_change, bool momentary);
systemState_StatusTypeDef system_layer_revert(void);
systemState_StatusTypeDef system_layer_get(uint8_t *layer);
systemState_StatusTypeDef system_layer_get_no_wait(uint8_t *layer);

#endif /* INC_SYSTEM_STATE_H_ */

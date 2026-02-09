/*
 * USB2533.h
 *
 *  Created on: May 26, 2024
 *      Author: bens1
 */

#ifndef INC_USB2533_H_
#define INC_USB2533_H_

#include "stm32g0xx_hal.h"
#include "stdint.h"

#define USB2533_I2C_CONFIG_ADDR (0x2D << 1)
#define USB2533_I2C_RUNTIME_ADDR (0x2C << 1)

#define USB2533_COMMAND_CONFIG_REG_ACCESS 0x9937
#define USB2533_COMMAND_USB_ATTACH 0xAA55
#define USB2533_COMMAND_OTP_PROGRAM 0x9933
#define USB2533_COMMAND_OTP_READ 0x9934

#define USB2533_

/* USB2533 Status typedef */
typedef enum {
	USB2533_OK 				= 0x00,
	USB2533_ERROR    		= 0x01,
	USB2533_BUSY    		= 0x02,
	USB2533_TIMEOUT 		= 0x03
} USB2533_StatusTypeDef;


/* USB2533 Device Struct */
typedef struct {

	/* I2C Handle*/
	I2C_HandleTypeDef *i2cHandle;

	uint8_t current_page;

} USB2533_HandleTypeDef;

/* Initialisation */
USB2533_StatusTypeDef USB2533_Init(USB2533_HandleTypeDef *dev, I2C_HandleTypeDef *i2cHandle);



USB2533_StatusTypeDef USB2533_ReadRegister(USB2533_HandleTypeDef *dev, uint8_t reg, uint8_t *data);
USB2533_StatusTypeDef USB2533_WriteRegister(USB2533_HandleTypeDef *dev, uint8_t reg, uint8_t *data);

USB2533_StatusTypeDef USB2533_WriteCommand(USB2533_HandleTypeDef *dev, uint16_t command);

#endif /* INC_USB2533_H_ */

/*
 * USB2533.c
 *
 *  Created on: May 26, 2024
 *      Author: bens1
 */

#include "USB2533.h"
#include "i2c.h"


USB2533_StatusTypeDef USB2533_Init(USB2533_HandleTypeDef *dev, I2C_HandleTypeDef *i2cHandle){

    USB2533_StatusTypeDef status = USB2533_OK;
//    uint8_t regData;

    dev->i2cHandle = i2cHandle;

    dev->current_page = 1;


    /* TODO: config here */

    /* USB Attach. Exit SOC_CONFIG and Enter HUB_CONFIG Stage. */
    status = USB2533_WriteCommand(dev, USB2533_COMMAND_USB_ATTACH);
    if (status != USB2533_OK){
    	return status;
    }

    return status;
}


//USB2533_StatusTypeDef USB2533_ReadRegister(USB2533_HandleTypeDef *dev, uint8_t reg, uint8_t *data);
//USB2533_StatusTypeDef USB2533_WriteRegister(USB2533_HandleTypeDef *dev, uint8_t reg, uint8_t *data);

USB2533_StatusTypeDef USB2533_WriteCommand(USB2533_HandleTypeDef *dev, uint16_t command){

    USB2533_StatusTypeDef status = USB2533_OK;
	uint8_t regData[3];

	regData[0] = command >> 8; 				   /* Command MSB */
	regData[1] = (uint8_t) (command & 0x00FF); /* Command LSB */
	regData[2] = 0x00; 						   /* Command complete */

    status = HAL_I2C_Master_Transmit(dev->i2cHandle, USB2533_I2C_CONFIG_ADDR, regData, 3, 100);

    return status;
}









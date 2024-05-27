/*
 * scan_keys_thread.c
 *
 *  Created on: May 19, 2024
 *      Author: bens1
 */

#include "threads.h"
#include "i2c.h"

#define PORT_A_PINS (KEY_C1_Pin | KEY_C2_Pin | KEY_C3_Pin | KEY_C4_Pin | KEY_D5_Pin | KEY_B1_Pin | KEY_B2_Pin | KEY_B3_Pin | KEY_D6_Pin | KEY_A6_Pin | KEY_B6_Pin)
#define PORT_B_PINS (KEY_B4_Pin | KEY_A1_Pin | KEY_C5_Pin | ENC_SW_Pin | KEY_E3_Pin | KEY_E2_Pin | KEY_A2_Pin | KEY_B5_Pin | KEY_A3_Pin)
#define PORT_C_PINS (KEY_A4_Pin | KEY_A5_Pin | KEY_E1_Pin | KEY_D1_Pin | KEY_D2_Pin)
#define PORT_D_PINS (KEY_C6_Pin)
#define PORT_F_PINS (KEY_D3_Pin | KEY_D4_Pin)

void scan_keys_thread(uint32_t initial_input){

	uint16_t keys_pressed_port_a = 0;
	uint16_t keys_pressed_port_b = 0;
	uint16_t keys_pressed_port_c = 0;
	uint16_t keys_pressed_port_d = 0;
	uint16_t keys_pressed_port_f = 0;

	uint16_t keys_pressed_port_a_prev = 0;
	uint16_t keys_pressed_port_b_prev = 0;
	uint16_t keys_pressed_port_c_prev = 0;
	uint16_t keys_pressed_port_d_prev = 0;
	uint16_t keys_pressed_port_f_prev = 0;

	while (1){
		// TODO: atomic load IDR
		keys_pressed_port_a = (GPIOA->IDR) & PORT_A_PINS;
		keys_pressed_port_b = (GPIOB->IDR) & PORT_B_PINS;
		keys_pressed_port_c = (GPIOC->IDR) & PORT_C_PINS;
		keys_pressed_port_d = (GPIOD->IDR) & PORT_D_PINS;
		keys_pressed_port_f = (GPIOF->IDR) & PORT_F_PINS;


		// TODO: Stuff goes here








		if (keys_pressed_port_a_prev != keys_pressed_port_a){
			HAL_GPIO_TogglePin(STAT1_LED_GPIO_Port, STAT1_LED_Pin);
		}

		keys_pressed_port_a_prev = keys_pressed_port_a;
		keys_pressed_port_b_prev = keys_pressed_port_b;
		keys_pressed_port_c_prev = keys_pressed_port_c;
		keys_pressed_port_d_prev = keys_pressed_port_d;
		keys_pressed_port_f_prev = keys_pressed_port_f;

		tx_thread_sleep(10); // TODO: Remove sleep and do proper timing
	}
}

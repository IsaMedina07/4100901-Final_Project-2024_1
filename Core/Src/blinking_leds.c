/*
 * blinking_leds.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */
#include "blinking_leds.h"
#include "main.h"


void blinking_on_led(void){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

uint8_t blinking_led_ret(uint8_t *toggles){

	static uint32_t tick = 0;
	if (tick < HAL_GetTick()) {
		if (*toggles > 0) {
			tick = HAL_GetTick() + 400;
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			(*toggles)--;
		} else {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			return 0;
		}
	}
	return 1;
}



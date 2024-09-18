/*
 * blinking_leds.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */
#include "blinking_leds.h"
#include "main.h"


uint8_t light_on(uint8_t *place){
	switch(place){
	case place == 1:
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
	break;
	case place == 2:
		HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
		break;
	case place == 3:
		HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
		break;
	case place == 4:
		HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
		break;
	}
}

uint8_t blinking_led_ret(uint8_t *toggles){

	static uint32_t tick = 0;
	if (tick < HAL_GetTick()) {
		if (*toggles > 0) {
			tick = HAL_GetTick() + 400;
			HAL_GPIO_TogglePin(Room_GPIO_Port, Room_Pin);
			HAL_GPIO_TogglePin(Living_GPIO_Port, Living_Pin);
			HAL_GPIO_TogglePin(Bathroom_GPIO_Port, Bathroom_Pin);
			HAL_GPIO_TogglePin(Kitchen_GPIO_Port, Kitchen_Pin);
			(*toggles)--;
		} else {
			HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_RESET);
			return 0;
		}
	}
	return 1;
}



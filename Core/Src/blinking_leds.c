/*
 * blinking_leds.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */
#include "blinking_leds.h"
#include "main.h"


uint8_t light_on(uint8_t place){

	switch(place){
	case 1:
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_RESET);
	break;
	case 2:
		HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_RESET);
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
			HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
			return 0;
		}
	}
	return 1;
}

void messages(void){
	printf("Welcome to your home!\r\n");
	printf("In which room do you want to be?\r\n");
	printf("Bedroom: Press A \r\n");
	printf("Living Room: Press B \r\n");
	printf("bathroom: Press C \r\n");
	printf("Kitchen: Press D \r\n");
}

void turn_off(uint8_t light_on){

#define LIGHTS (30 * 1000) // 30 segundos
	static uint32_t time_on = LIGHTS;

	if (time_on> HAL_GetTick()) {
		return;
	}

	switch (light_on){

	case 1:
		printf("turning off\r\n",light_on);
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
	break;
	case 2:
		printf("turning off\r\n",light_on);
		HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
		break;
	case 3:
		printf("turning off\r\n",light_on);
		HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
		break;
	case 4:
		printf("turning off\r\n",light_on);
		HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
		break;

	}
	time_on = HAL_GetTick() + LIGHTS;


}



/*
 * blinking_leds.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */
#include "blinking_leds.h"
#include "main.h"

//variables para el control de las luces de cada habitación

uint32_t Bedroom_on = 0;
uint32_t Living_on = 0;
uint32_t Bathroom_on = 0;
uint32_t Kitchen_on = 0;

//función para el encendido de las luces según la habitación que corresponda
void light_on(uint8_t place){

	switch(place){
	case 1:
		Bedroom_on = HAL_GetTick();
		printf("Bedroom\r\n");
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_RESET);
	break;
	case 2:
		Living_on = HAL_GetTick();
		printf("Livingroom\r\n");
		HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		Bathroom_on = HAL_GetTick();
		printf("Bathroom\r\n");
		HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		Kitchen_on = HAL_GetTick();
		printf("Kitchen\r\n");
		HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_RESET);
		break;
	}
}

//función para el parpadeo de todos los leds en caso de que la clave sea incorrecta
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

//mensajes de bienvenida
void messages(void){
	printf("Welcome to your home!\r\n");
	printf("In which room do you want to be?\r\n");
	printf("Bedroom: Press A \r\n");
	printf("Living Room: Press B \r\n");
	printf("bathroom: Press C \r\n");
	printf("Kitchen: Press D \r\n");
}

//función para apagar las luces después de un tiempo determinado
void turn_off(void){

	if(HAL_GetTick() > Bedroom_on + WAITING_TIME){
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
	}
	if(HAL_GetTick() > Living_on + WAITING_TIME){
			HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
		}
	if(HAL_GetTick() > Bathroom_on + WAITING_TIME){
			HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
		}
	if(HAL_GetTick() > Kitchen_on + WAITING_TIME){
			HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
		}
}



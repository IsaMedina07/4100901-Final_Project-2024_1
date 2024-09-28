/*
 * blinking_leds.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */
#include "blinking_leds.h"
#include "main.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

//variables para el control de las luces de cada habitación

uint32_t Bedroom_on = 0xFFFF0000;
uint32_t Living_on = 0xFFFF0000;
uint32_t Bathroom_on = 0xFFFF0000;
uint32_t Kitchen_on = 0xFFFF0000;




//función para el encendido de las luces según la habitación que corresponda
uint8_t light_on(uint8_t place){

	uint8_t place_on = 0;

	switch(place){
	case 1:
		Bedroom_on = HAL_GetTick();
		printf("Bedroom\r\n");
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_RESET);
		place_on = 1;
	break;
	case 2:
		Living_on = HAL_GetTick();
		printf("Livingroom\r\n");
		HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_RESET);
		place_on = 2;
		break;
	case 3:
		Bathroom_on = HAL_GetTick();
		printf("Bathroom\r\n");
		HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_RESET);
		place_on = 3;
		break;
	case 4:
		Kitchen_on = HAL_GetTick();
		printf("Kitchen\r\n");
		HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_RESET);
		place_on = 4;
		break;
	}
	return place_on;
}

//función para el parpadeo de todos los leds en caso de que la clave sea incorrecta
uint8_t blinking_led_ret(uint8_t *toggles){

	static uint32_t tick = 0;
	if (tick < HAL_GetTick()) {
		if (*toggles > 0) {
			tick = HAL_GetTick() + 500;
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
	printf("--In which room do you want to be?--\r\n");
	printf("Bedroom: Press A \r\n");
	printf("Living Room: Press B \r\n");
	printf("bathroom: Press C \r\n");
	printf("Kitchen: Press D \r\n");
}


//función para apagar las luces después de un tiempo determinado
void turn_off_with_time(void){


	 if(HAL_GetTick() > Bedroom_on + WAITING_TIME){
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
		Bedroom_on = 0xFFFF0000;
	 }
		if(HAL_GetTick() > Living_on + WAITING_TIME){
			HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
			Living_on = 0xFFFF0000;
		}

		if(HAL_GetTick() > Bathroom_on + WAITING_TIME){
			HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
			Bathroom_on = 0xFFFF0000;
		}

	if(HAL_GetTick() > Kitchen_on + WAITING_TIME){
			HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
			Kitchen_on = 0xFFFF0000;
		}


}

//función para apagar todos los leds
void turn_off(uint8_t turn_off_light){

	switch (turn_off_light){
	case 1:
		HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
	break;

	case 2:
			HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);
	break;

	case 3:
			HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
	break;
	case 4:
			HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
	break;
	}
}

//función para apagar todas las luces
void turn_off_completely(void){
	HAL_GPIO_WritePin(Kitchen_GPIO_Port, Kitchen_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Bathroom_GPIO_Port, Bathroom_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Room_GPIO_Port, Room_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Living_GPIO_Port, Living_Pin, GPIO_PIN_SET);

}



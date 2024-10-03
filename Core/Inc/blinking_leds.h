/*
 * blinking_leds.h
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */

#ifndef INC_BLINKING_LEDS_H_
#define INC_BLINKING_LEDS_H_
#define WAITING_TIME 5000 //tiempo de espera para el apagado de las luces
#include "main.h"

#include <stdint.h>

uint8_t light_on(uint8_t place, UART_HandleTypeDef *huart);
uint8_t blinking_led_ret(uint8_t *toggles);
void messages(void);
//void turn_off(uint8_t light_on);
void turn_off_with_time(void);
void turn_off(uint8_t turn_off_light, UART_HandleTypeDef *huart);
uint8_t party_lights(void);
#endif /* INC_BLINKING_LEDS_H_ */

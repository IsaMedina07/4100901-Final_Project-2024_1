/*
 * blinking_leds.h
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */

#ifndef INC_BLINKING_LEDS_H_
#define INC_BLINKING_LEDS_H_

#include <stdint.h>

uint8_t light_on(uint8_t place);
uint8_t blinking_led_ret(uint8_t *toggles);

#endif /* INC_BLINKING_LEDS_H_ */

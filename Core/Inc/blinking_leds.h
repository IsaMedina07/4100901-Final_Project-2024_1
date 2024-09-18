/*
 * blinking_leds.h
 *
 *  Created on: Sep 18, 2024
 *      Author: Paulina Ruíz
 */

#ifndef INC_BLINKING_LEDS_H_
#define INC_BLINKING_LEDS_H_

#include <stdint.h>

void blinking_led(void);
uint8_t blinking_led_ret(uint8_t *toggles);

#endif /* INC_BLINKING_LEDS_H_ */

/*
 * API_led.h
 *
 *  Created on: 9 abr 2026
 *      Author: vicks
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

#include <stdint.h>
#include <stdbool.h>
typedef enum {
	LED_OFF = 0, LED_ON, LED_TOGGLE
} led_state_t;

void ledOn(void);
void ledOff(void);
void ledToggle(uint32_t tiempo);
void ledUpdate();
void ledInit(void);




#endif /* API_INC_API_LED_H_ */

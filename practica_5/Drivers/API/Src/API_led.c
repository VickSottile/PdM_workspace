/*
 * API_led.c
 *
 *  Created on: 9 abr 2026
 *      Author: vicks
 */

#define LED_DELAY 500 //delay del parpadeo

#include "API_led.h"
#include "API_delay.h"
#include "API_cmdparser.h"
#include "stm32f4xx_hal.h"
#include "main.h"



static bool_t ready=false;
static led_state_t led=LED_OFF;

//Variables internas del módulo
static delay_t ledDelay;




void ledOn(void){			// debe encender el LED

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	led = LED_ON;
	return;
}
void ledOff(void){		// debe apagar el LED
	led = LED_OFF;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	return;
}

void ledUpdate(){
	if (led==LED_TOGGLE){
		ready=delayRead(&ledDelay);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
	if (led==LED_OFF){
		ledOff();
	}
	if (led==LED_ON){
			ledOn();
		}



}

void ledToggle(uint32_t tiempo){
	led = LED_TOGGLE;
	delayInit(&ledDelay, tiempo);
}

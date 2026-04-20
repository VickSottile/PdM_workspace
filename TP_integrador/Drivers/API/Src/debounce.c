/*
 * API_debounce.c
 *
 *  Created on: 30 mar 2026
 *      Author: vicks
 */
#include <debounce.h>
#include <stdbool.h>
#include "board_port.h"
#define DEBTIME 40

tick_t debounce;
static debounceState_t estadoActual; // variable que refleja el estado de la MEF del debounce
static bool_t keypressed; // se pone en true cuando ocurre un flanco descendente y se pone en false cuando se llame a la función readKey()

static void buttonPressed(void){			// debe encender el LED

	ledOn();
}
static void buttonReleased(void){		// debe apagar el LED
	ledOff();
}


void debounceFSM_init() //  carga el estado inicial
{
	estadoActual = BUTTON_UP;
	buttonReleased();
	}
void debounceFSM_update()	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas
{
	switch (estadoActual){
		case BUTTON_UP:
			if(GPIOState() == GPIO_PIN_RESET){
				estadoActual=BUTTON_FALLING;
				debounce=getTick();
			}
				break;
		case BUTTON_FALLING:
			if((getTick() - debounce) >= DEBTIME){
				if(GPIOState() == GPIO_PIN_RESET){
				estadoActual=BUTTON_DOWN;
				keypressed=true;

			}else{
				estadoActual=BUTTON_UP;
			}}
				break;
		case BUTTON_DOWN:
			if(GPIOState()== GPIO_PIN_SET){
				estadoActual=BUTTON_RISSING;
				debounce=getTick();
				buttonPressed();
				//Aca deberia iniciar un contador llamar a una funcion o solo usar una variable y guardar el tiempo de la Hall como en BUTTON_UP
			}
				break;
		case BUTTON_RISSING:
			//Aca la misa logica que en BUTTON_FALLING
			if((getTick() - debounce) >= DEBTIME){
			if(GPIOState() == GPIO_PIN_SET){
				estadoActual=BUTTON_UP;

				//buttonReleased();
			}else{
				estadoActual=BUTTON_DOWN;
			}}
				break;
		default:
			debounceFSM_init();
				break;
	}
}


bool_t readKey(){
	bool_t key=keypressed;

	keypressed=false;

	return key;
}

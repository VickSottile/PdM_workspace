/*
 * API_debounce.c
 *
 *  Created on: 30 mar 2026
 *      Author: vicks
 */
#include "API_debounce.h"
#include <stdbool.h>
#define DEBTIME 40

tick_t debounce;
static debounceState_t estadoActual; // variable que refleja el estado de la MEF
static bool_t keypressed; // se pone en true cuando ocurre un flanco descendente y se pone en false cuando se llame a la función readKey()



void debounceFSM_init() //  carga el estado inicial
{
	estadoActual = BUTTON_UP;
	buttonReleased();
	}
void debounceFSM_update()	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas
{
	switch (estadoActual){
		case BUTTON_UP:
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
				estadoActual=BUTTON_FALLING;
				debounce=HAL_GetTick();
			}
				break;
		case BUTTON_FALLING:
			if((HAL_GetTick() - debounce) >= DEBTIME){
				//Aca antes del if tengo que hacer otro if y preguntar si el tiempo es mayor a 40ms comparando la variable y ahi recien hacer lo que sigue
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
				estadoActual=BUTTON_DOWN;
				keypressed=true;
				//buttonPressed();
			}else{
				estadoActual=BUTTON_UP;
			}
				break;
		case BUTTON_DOWN:
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
				estadoActual=BUTTON_RISSING;
				debounce=HAL_GetTick();
				//Aca deberia iniciar un contador llamar a una funcion o solo usar una variable y guardar el tiempo de la Hall como en BUTTON_UP
			}}
				break;
		case BUTTON_RISSING:
			//Aca la misa logica que en BUTTON_FALLING
			if((HAL_GetTick() - debounce) >= DEBTIME){
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
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

static void buttonPressed(void){			// debe encender el LED

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}
static void buttonReleased(void){		// debe apagar el LED
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}
bool_t readKey(){
	bool_t key=keypressed;
	keypressed=false;
	return key;
}

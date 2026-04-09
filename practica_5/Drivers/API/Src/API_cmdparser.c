/*
 * API_cmdparser.c
 *
 *  Created on: 7 abr 2026
 *      Author: vicks
 */



#define CMD_MAX_LINE 64//incluye '/0'
#define CMD_MAX_TOKENS 3// COMANDO + 2 argumentos
#define BLINK_FREC 500 //frecuencia de parpadeo

#include <stdio.h>
#include "API_cmdparser.h"
#include "API_uart.h"
#include <string.h>
#include <stdlib.h>
#include "API_led.h"

//estados de la MEF UART
typedef enum {
	CMD_IDLE = 0, CMD_RECEIVING, CMD_ERROR, CMD_PROCESS, CMD_EXEC
} cmd_state_t;



cmd_state_t state = CMD_IDLE; //state guarda el estado de la MEF, inicializa en CMD_IDLE
cmd_status_t status = CMD_OK;
led_state_t led=LED_OFF;

uint8_t buffer[CMD_MAX_LINE];
static uint8_t i = 0; //index del buffer
static uint8_t comment = 0; //bandera para detectar comentarios que son los que empiezan con # o //

//Prototipo de funciones públicas

//Inicializa el módulo parser de comandos
void cmdParserInit(void) {
	uartInit();

}

cmd_status_t cmdProcessLine() {

	//llamado a ayuda
	if(strcmp((char*)buffer,"HELP")==0)
	{
		cmdPrintHelp();
		return CMD_OK;
	}
	//Encender LED
	if(strcmp((char*)buffer,"LED ON")==0)
	{
		uartSendString((uint8_t*)"Voy a encender el led\r\n");
		//encencer led
		ledOn();
		led=LED_ON;

		return CMD_OK;
	}
	 //Apagar LED
	if(strcmp((char*)buffer,"LED OFF")==0){
		//apagar led
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		led=LED_OFF;
		ledOff();
		return CMD_OK;
	}

	if(strcmp((char*)buffer,"LED TOGGLE")==0){
		//llamar a parpadear LED desde API_DEBOUNCE o el API_DELAYC

		led=LED_TOGGLE;
		ledToggle(BLINK_FREC);
		return CMD_OK;
	}

	if(strcmp((char*)buffer,"STATUS")==0){
			switch(led)
			{
			case LED_OFF:
				uartSendString((uint8_t*)"LED is OFF\r\n");
				break;
			case LED_ON:
				uartSendString((uint8_t*)"LED is ON\r\n");
				break;
			case LED_TOGGLE:
				uartSendString((uint8_t*)"LED is TOGGLING\r\n");
				break;
			default:
				uartSendString((uint8_t*)"ERROR: unknown command\r\n");
				return CMD_ERR_UNKNOWN;
			}
			return CMD_OK;




		}
	memset(buffer, 0, CMD_MAX_LINE);



}

// Maquina de estados del parser. Debe ser llamada períodicamente desde el bucle
// Procesa hasta 16 bytes por invocación (no bloqueante).
void cmdPoll(void) {
	uint8_t dato;

	uartReceiveStringSize(&dato, 1);



	switch (state) {
	case CMD_IDLE:

		if (i == 0) {
			if (dato == '#') {
				comment = 1; //bandera en 1 es un comentario

			} else {
				buffer[i] = dato;
				if (dato == '/') {
					comment = 2; //bandera en 2 puede ser un comentario, esperar sgte valor
				} else {
					comment = 0; //bandera en 0 es una linea normal
				}
			}
		}

		state = CMD_RECEIVING;

		break;

	case CMD_RECEIVING:

		//detección de comentario tipo "//"
		//uartSendString((uint8_t*)"Estoy en Receiving\r\n");
		if (!uartDataAvailable()) {
		        return; // solo salgo si NO hay dato
		    }
		uartSendString((uint8_t*)"CMD: ");
		uartSendString((uint8_t*)buffer);
		uartSendString((uint8_t*)"\r\n");

		if ((i == 1) && (comment == 2) && (dato == '/')) {
			comment = 1;//si el primer caracter fue una / y el segundo tmb es un comentario
		}
		if ((i < CMD_MAX_LINE - 1) && (comment != 1)) {
			if ((dato != '\n') && (dato != '\r')) {
				buffer[i++] = dato;

			} else //caracter de terminacion
			{
			    if(i == 0){
			        // si la linea esta vacia la ignora
			        state = CMD_IDLE;
			    } else {
			        buffer[i] = '\0';
			        i = 0;
			        state = CMD_PROCESS;
			        //uartSendString((uint8_t*)"Cambiar a CMDPROCESS\r\n");
			        //chequeo lo que voy a procesar
			        uartSendString((uint8_t*)"CMD: ");
			        uartSendString((uint8_t*)buffer);
			        uartSendString((uint8_t*)"\r\n");
			    }
			}
		} else { //overflow
			i = 0;
			state = CMD_ERROR;
			status = CMD_ERR_OVERFLOW;
		}


		break;
	case CMD_PROCESS:
		uartSendString((uint8_t*)"Estoy en CMDPROCESS\r\n");
		 uartSendString((uint8_t*)"CMD: ");
		 uartSendString((uint8_t*)buffer);
		 uartSendString((uint8_t*)"\r\n");
		status = cmdProcessLine();
		state = CMD_IDLE;
		break;

	case CMD_ERROR:
		status = CMD_ERR_OVERFLOW;
		i = 0;
		state = CMD_IDLE;
		break;
	case CMD_EXEC:
		state = CMD_IDLE;
		break;
	default:
		state = CMD_IDLE;
		break;
	}
	return;
}




//Imprime por UART la lista de comandos disponibles
void cmdPrintHelp(void) {
	return;
}



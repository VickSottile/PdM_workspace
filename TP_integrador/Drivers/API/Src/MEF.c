/*
 * MEF.c
 *
 * Maquina de estados principal del sistema de medición de suelos.
 *  Flujo de estados:
 *  INIT → IDLE → REQ_SENSING → SENSING → SHOW → IDLE
 *                         → ERROR1 → INIT
 *  Salidas:
 *    - UART2  (debug / terminal serie)
 *    - LCD 16x2 por I2C
 *
 *  Created on: 11 abr 2026
 *      Author: vicks
 */

#include "MEF.h"

/*Tiempos en milisegundos*/
#define WAITTIME 5000
#define WAITTIME2 1000
#define TOGGLESENSING 500
#define TOGGLEIDLE 500

/*Longitud máxima del buffer de mensajes para UART*/
#define MSG_LEN 80

/*Variables privadas de la MEF*/
static tick_t time=0;
static bool_t button=false;
static bool_t flag=false;
static MEFState_t mef = IDLE;
static MEFSensorState_t soil;

/*buffer para armar el mensaje de resultado*/
static char resultMsg[MSG_LEN];


/*Mostrar datos por UART2 y LCD
 * utilizo const para que los datos no puedan ser modificados dentro de la funcion
 * */

static void showSensorData (const soilsensor_data_t *d){
	char buffer [MSG_LEN];

	//envio de datos por UART para debug
	sprintf(buffer, "Humedad: %.1f !!\r\n", d->humedad);
	uartSentString((uint8_t*)buffer);


}

void MEF_init(void) {
	uartInit();
	soilSensorInit();
	mef = INIT;

}




void MEFUpdate (void) {
	MEF_soilSensorUpdate();
	switch(mef){
	case INIT:
		if (flag==false){
		uartSendString((uint8_t*)"Bienvenidos al sistema de Medicion de Suelos\r\n");
		flag=true;
		}

		time=getTick();

		mef=IDLE;
		flag=false;
		break;

	case IDLE:
		button=readKey();
		if (button==true)
			{
				mef=REQ_SENSING;
				button=false;

			}
		if((getTick() - time) >= WAITTIME){
			if (flag==false){
			uartSendString((uint8_t*)"Presione el pulsador para iniciar\r\n");
			flag=true;

			ledToggle(TOGGLEIDLE);
				}

			}


		break;
	case REQ_SENSING:
		requestSoilData();
		mef=SENSING;
		break;
	case SENSING:
		soil= requestSoilDataState();
		if (soil==ERROR485){
			uartSendString((uint8_t*)"ERROR Obteniendo datos\r\n");
			mef=ERROR1;
			break;
		}
		if(soil!=DONE485){
		uartSendString((uint8_t*)"Midiendo, Aguarde\r\n");
		ledToggle(TOGGLESENSING);
		}
		else
		{
			mef=SHOW;
			ledOff();
			time=getTick();
		}
		break;
	case SHOW:
		uartSendString((uint8_t*)"La medicion del suelo es\r\n");
		if((getTick() - time) >= WAITTIME2){
			mef=IDLE;
				}
		break;
	case ERROR1:
		uartSendString((uint8_t*)"Error en la medicion, reiniciando\r\n");
		mef=INIT;
		break;
	default:
		uartSendString((uint8_t*)"Error desconocido, reiniciando\r\n");
		mef=INIT;
		break;
	}
	}


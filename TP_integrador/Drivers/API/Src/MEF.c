/*
 * MEF.c
 *
 *  Created on: 11 abr 2026
 *      Author: vicks
 */

#include "MEF.h"
#define WAITTIME 5000
#define WAITTIME2 1000
#define TOGGLESENSING 500
#define TOGGLEIDLE 500

static tick_t time=0;
bool_t button=false;
bool_t dataReady= false;
static bool_t flag=false;
MEFState_t mef = IDLE;

void MEF_init(void) {
	uartInit();
	mef = INIT;

}


void MEFUpdate (void) {
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
				mef=SENSING;
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
	case SENSING:
		if(!dataReady){
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

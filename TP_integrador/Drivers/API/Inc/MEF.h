/*
 * MEF.h
 *
 *  Created on: 11 abr 2026
 *      Author: vicks
 */

#ifndef API_INC_MEF_H_
#define API_INC_MEF_H_
#include <stdbool.h>
#include <API_debounce.h>
#include "board_port.h"


typedef enum{
    INIT=0,
	IDLE,
	SENSING,
	SHOW,
	ERROR1
} MEFState_t;

/* Actualización de la máquina de estados*/
void MEFUpdate (void);

/*Inicializacion de la MEF*/
void MEF_init(void);

#endif /* API_INC_MEF_H_ */

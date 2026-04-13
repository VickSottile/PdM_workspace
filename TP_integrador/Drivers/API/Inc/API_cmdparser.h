/*
 * API_cmdparser.h
 *
 *  Created on: 7 abr 2026
 *      Author: vicks
 */

#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_



typedef enum{
	CMD_OK=0,
	CMD_ERR_OVERFLOW,
	CMD_ERR_SYNTAX,
	CMD_ERR_UNKNOWN,
	CMD_ERR_ARG
} cmd_status_t;


//Prototipo de funciones públicas

//Inicializa el módulo parser de comandos
void cmdParserInit(void);


// Maquina de estados del parser. Debe ser llamada períodicamente desde el bucle
// Procesa hasta 16 bytes por invocación (no bloqueante).
void cmdPoll(void);


//Imprime por UART la lista de comandos disponibles
void cmdPrintHelp(void);

#endif /* API_INC_API_CMDPARSER_H_ */

/*
 * uart_rs485.c
 *
 *  Created on: 19 abr 2026
 *      Author: vicks
 *  About: Capa de abstracción de hardware RS485
 */


#include "uart_rs485.h"
#include <stdbool.h>
#include "board_port.h"

#define TIMEOUT_UART2 50
#define TIMEOUT_BYTE 2
#define RS485_DE_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_8



static UART_HandleTypeDef huart1;

/*Habilitación Pin para transmitir*/
void enableTransmit(void){
	 HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
}

/*Habilitacion del Pin para recibir*/
void enableReceive(void){
	 HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
}


static  bool_t uartRxReady=false; // variable interna que refleja si hay datos para leer

HAL_StatusTypeDef transmitido485;
HAL_StatusTypeDef recibido485;

bool_t uart485Init(){
	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = 4800;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
	    Error_Handler();
	    return false;
	  }
	  else {
		  /* Configuracion GPIO DE/RE Pin RS485*/
		  GPIO_InitTypeDef GPIO_InitStruct = {0};
		    GPIO_InitStruct.Pin = RS485_DE_Pin;
		    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		    GPIO_InitStruct.Pull = GPIO_NOPULL;
		    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		    HAL_GPIO_Init(RS485_DE_GPIO_Port, &GPIO_InitStruct);

		  return true;
	  }


}

rs485status_t uart485Transmit(const uint8_t *data, uint16_t len){
	if(data==NULL||len==0){
		return RS485_ERROR;
	}
	enableTransmit();
	transmitido485 = HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY);
	//Espero a que termine de enviar usando la Flag de la Hal
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==RESET);
	//cuando termina de recibir habilito la recepción
	enableReceive();
	if(transmitido485!=HAL_OK){
		return RS485_ERROR;
	}else{
		return RS485_OK;
	}

}
/*Recibir de a un byte hasta completar la trama con timeout minimo*/
rs485status_t uart485ReceiveByte(uint8_t *byte){
	if(byte==NULL){
		return RS485_ERROR;
	}
	recibido485 = HAL_UART_Receive(&huart1,byte,1,TIMEOUT_BYTE);
	if (recibido485 != HAL_OK) {
		return RS485_TIMEOUT;
	}else{
		return RS485_OK;
	}
}


rs485status_t uart485SendString(uint8_t *pstring) //envia el  String
{
	if (pstring!=NULL)
	{	enableTransmit();
		transmitido485 = HAL_UART_Transmit(&huart1, pstring, strlen((char*)pstring),TIMEOUT_UART2);
		if (transmitido485 != HAL_OK)
			{

				return RS485_ERROR;
			}

	}

		return RS485_OK;
}

void uart485ReceiveStringSize(uint8_t * pstring, uint16_t size) // Recibe el tamaño del string
{
	uartRxReady = false;
	if (pstring!=NULL)
		{
		transmitido485 =HAL_UART_Receive (&huart1, pstring, size,TIMEOUT_UART2);
		if (transmitido485== HAL_ERROR){
			Error_Handler();
		}
		if (transmitido485 == HAL_TIMEOUT){
			uartRxReady = false; // Saque el errorhandler porque se colgaba al llamarlo por timeout
		}else{
			uartRxReady = true; // significa que hay datos recibidos disponibles
		}
		}
	return;
}

bool_t uart485DataAvailable(void)
{
    return uartRxReady;
}

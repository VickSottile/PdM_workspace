/*
 * API_uart.c
 *
 *  Created on: 2 abr 2026
 *      Author: vicks
 */
#include "API_uart.h"


UART_HandleTypeDef huart2;

HAL_StatusTypeDef transmitido;


//Funciones
bool_t uartInit(){ //inicializa la UART
	  huart2.Instance = USART2;
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  Error_Handler();
		  return false;

	  } else {
		    char buffer[200];

		    sprintf(buffer,
		        "UART Config:\r\n"
		        "BaudRate: %lu\r\n"
		        "WordLength: %d\r\n"
		        "StopBits: %d\r\n"
		        "Parity: %d\r\n",
		        huart2.Init.BaudRate,
		        huart2.Init.WordLength,
		        huart2.Init.StopBits,
		        huart2.Init.Parity
		    );

		    uartSendStringSize((uint8_t*)buffer, strlen(buffer));
		  return true;
		  //Implementar envío de configuración
	  }

}
void uartSendString(uint8_t *pstring) //envia el  String
{
	if (pstring!=NULL)
	{
		transmitido = HAL_UART_Transmit(&huart2, pstring, strlen((char*)pstring),100);
		if (transmitido != HAL_OK)
			{
				Error_Handler();
				return;
			}

	}
		//Se podria implementar un parpadeo de leds con codigo de error
		return;
}
void uartSendStringSize(uint8_t * pstring, uint16_t size) {//envía el tamaño del string
	//size entre 1 y 256 (valor razonable que se puede ajustar)
	if ((size>=1 && size<=256)&&(pstring!=NULL))
	{
	transmitido = HAL_UART_Transmit(&huart2, pstring,size,100);
	if (transmitido != HAL_OK)
	{
		Error_Handler();
	}
	}
	//Se podria implementar un parpadeo de leds con codigo de error
	return;
}
void uartReceiveStringSize(uint8_t * pstring, uint16_t size) // Recibe el tamaño del string
{
	HAL_UART_Receive (&huart2, pstring, size,100);
}


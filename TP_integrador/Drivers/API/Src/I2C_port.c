/*
 * I2C_port.c
 *
 *  Created on: 13 abr 2026
 *      Author: vicks
 */

#include "I2C_port.h"
#define ADDRESS 0x27


static uint8_t data;

I2C_HandleTypeDef hi2c1;

void I2Cinit() {

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

 void I2CWriteByte(uint8_t data)
{
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS <<1, &data, 1, HAL_MAX_DELAY);
	}

 void I2CReadByte(){
	HAL_I2C_Master_Receive(&hi2c1, ADDRESS <<1, &data, 1, HAL_MAX_DELAY);
}

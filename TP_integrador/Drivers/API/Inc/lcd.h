/*
 * API_lcd.h
 *
 *  Created on: 10 abr 2026
 *      Author: vicks
 */

#ifndef API_INC_LCD_H_
#define API_INC_LCD_H_

#include "I2C_port.h"

void LCD_init(void);
void LCD_clear(void);
void LCD_showWelcome(void);
void LCD_writeString(char * str);
void LCD_delay(uint8_t timeD);
//void LCD_send_nibble(uint8_t nibble, uint8_t rs);
//void LCD_send_byte(uint8_t byte, uint8_t rs);




// void LCD_showData(soil_data_t data);


#endif /* API_INC_LCD_H_ */

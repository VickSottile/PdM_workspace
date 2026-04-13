/*
 * API_lcd.h
 *
 *  Created on: 10 abr 2026
 *      Author: vicks
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_



void LCD_init(void);
void LCD_clear(void);
void LCD_showWelcome(void);
void LCD_writeString(char * str);
//void LCD_showData(soil_data_t data);


#endif /* API_INC_API_LCD_H_ */

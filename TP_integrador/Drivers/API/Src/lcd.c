/*
 * API_lcd.c
 *
 *  Created on: 10 abr 2026
 *      Author: vicks
 */

#include <lcd.h>

#define MODO_4BIT 0x28
#define RETURN_HOME 0x02
#define ENTRY_MODE 0x04
#define AUTOINCREMENT 0x02
#define CONTROL_DISPLAY 0x08
#define DISPLAY_ON 0x04
#define CLEAR_LCD 0x01

static uint8_t LCD_INIT_CMD[]= {MODO_4BIT, CONTROL_DISPLAY, RETURN_HOME, ENTRY_MODE|AUTOINCREMENT,
		CONTROL_DISPLAY|DISPLAY_ON,CLEAR_LCD};


static void LCD_send_nibble(uint8_t nibble, uint8_t rs)
{

	uint8_t data=0;
	data = (nibble<<4);
	if(rs){
		data|=(1<<0);
	}
	data|=(1<<3); //Enciende el backlight
	data|=(1<<2); // para que el LCD lea los datos necesita pasar el EN de 1 a 0
	I2CWriteByte(data);
	LCD_delay(1);

	data &=~(1<<2);
	I2CWriteByte(data);
	LCD_delay(1);
}

static void LCD_send_byte(uint8_t byte, uint8_t rs){
	LCD_send_nibble(byte >> 4, rs); //envía el nibble alto
	LCD_send_nibble(byte & 0x0F, rs);  // envia el nibble bajo
}


static void LCD_write_command(uint8_t cmd)
{
    LCD_send_byte(cmd, 0); // RS = 0
    LCD_delay(2);
}

static void LCD_write_data(uint8_t data)
{
    LCD_send_byte(data, 1); // RS = 1
    LCD_delay(2);
}

void LCD_delay(uint8_t timeD){
	I2CDelay(timeD);
}






void LCD_clear(){

}
void LCD_showWelcome(){

}
void LCD_writeString(char * str){
	 while(*str)   // mientras no sea '\0'
	    {
	        LCD_write_data((uint8_t)*str);
	        str++;
	    }

}

void LCD_init(){
	LCD_delay(20);
	LCD_send_nibble(0x03, 0);
	LCD_delay(10);
    LCD_send_nibble(0x03, 0);
    LCD_delay(1);
    LCD_send_nibble(0x03, 0);
    LCD_delay(1);
    LCD_send_nibble(0x02, 0);
    uint8_t size = sizeof(LCD_INIT_CMD)/sizeof(LCD_INIT_CMD[0]);
    for( uint8_t i=0; i<size; i++)
    {
    	LCD_write_command(LCD_INIT_CMD[i]);
    }


}

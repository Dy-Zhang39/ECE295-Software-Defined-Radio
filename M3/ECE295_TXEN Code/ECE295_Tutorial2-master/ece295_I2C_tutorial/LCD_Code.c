/*
 * ece295_I2C_tutorial.c
 *
 * Created: 9/2/2021 5:38:25 PM
 * Author : amack
 */ 

#define F_CPU		8000000UL // Clock speed
#define SCREEN_ADDR 0x78

#include <avr/io.h>
#include <util/delay.h> //for delay function
#include <stdio.h>

#include "i2c.h"
#include "screen_cmds.h"

void screen_write_string(char string_to_write[], int line_num)
{
	int letter=0;

	I2Csendcmd(SCREEN_ADDR, line_num);
	int current_line = line_num;
	
	for(letter = 0; string_to_write[letter] != 0; letter++)	{
		if ((letter != 0) && (letter % LINE_LENGTH == 0))
		{
			if (current_line == COMMAND_SET_CURSOR_LINE_4){
				current_line = line_num;// We've gone past the end of the line, go to the next one
				I2Csendcmd(SCREEN_ADDR, COMMAND_CLEAR_DISPLAY);
			}
			else
			current_line = current_line+0x20;
			I2Csendcmd(SCREEN_ADDR, current_line); // We've gone past the end of the line, go to the next one
		}
		I2Csenddatum(SCREEN_ADDR, string_to_write[letter]);
	}
}

int main(void)
{
	DDRA = 0xff; //PortA as output (only need PA0 for display)
	
	_delay_ms(5);
	PORTA = PORTA | (0<<PA0); // turn off
	_delay_ms(200);
	PORTA = PORTA | (1<<PA0); // turn on display
	_delay_ms(5);
	
	//Set up I2C
	I2Cinit();
	//I2Cstart();
	
	//Initialize display
	I2Csendcmd(SCREEN_ADDR, 0x01); // clear display
	I2Csendcmd(SCREEN_ADDR, 0x3A);
	I2Csendcmd(SCREEN_ADDR, 0x09);
	I2Csendcmd(SCREEN_ADDR, 0x06);
	I2Csendcmd(SCREEN_ADDR, 0x1E);
	I2Csendcmd(SCREEN_ADDR, 0x39);
	I2Csendcmd(SCREEN_ADDR, 0x1B);
	I2Csendcmd(SCREEN_ADDR, 0x6E);
	I2Csendcmd(SCREEN_ADDR, 0x57);
	I2Csendcmd(SCREEN_ADDR, 0x7A);
	I2Csendcmd(SCREEN_ADDR, 0x38);
	I2Csendcmd(SCREEN_ADDR, 0x0F);
	I2Csendcmd(SCREEN_ADDR, 0x01); // clear display
	
    //Write some data
	screen_write_string("FREQ: 300M", COMMAND_SET_CURSOR_LINE_1);
	screen_write_string("MODE: RX", COMMAND_SET_CURSOR_LINE_2);
}


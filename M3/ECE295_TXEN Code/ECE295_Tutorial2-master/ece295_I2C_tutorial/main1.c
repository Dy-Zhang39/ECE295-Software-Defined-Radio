/*
 * Team50_SDR_Keypad.c
 *
 * Created: 3/29/2022 10:06:10 AM
 * Author : ECE295_Student
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#define TRUE 0x1
#define FALSE 0x0

#define ROW1 0x1
#define ROW2 0x2
#define ROW3 0x4
#define ROW4 0x8
#define COL1 0x1
#define COL2 0x2
#define COL3 0x4
#define COL4 0x8

#define KEY_A 0xa
#define KEY_B 0xb
#define KEY_C 0xc
#define KEY_D 0xd
#define KEY_HASHTAG 0xe
#define KEY_ASTERISK 0xf
#define KEY_UNKNOWN 0x10

int key_row = 0;
int key_col = 0;

int input_char_pos = 0;
int is_entering = FALSE;
int freq = 0;
int freq_temp = 0;

int TXEN = FALSE;



int main(void)
{
    /* Replace with your application code */
	DDRB = 0xf0; //Set ports B 4-7 as output, 0-3 as input
	DDRD = 0x3c; //Set port D as output for test LED
	
	PCICR = 0x2; //Setting PCICR (ATMEGA's GIC) to enable interrupts from PCINT1
	PCMSK1 = 0xf; //Setting interrupt masks of pins PCINT8-11 to 1
	sei(); //Enable interrupts by setting I bit of the Status Register to 1
	
	volatile uint8_t PB_value;
	volatile uint8_t PD_value;
    while (1) 
    {
		PB_value = PINB;
		PD_value = PIND;
		PORTB = 0xf0; //ToDo will this matter if PORTB[3:0] are set to input?
		// ToDo Store and restore SREG
    }
}

ISR(PCINT1_vect){
	int input_char = KEY_UNKNOWN;
	
	_delay_ms(100);
	/*if ((PINB & 0xf) != 0){
		PORTD = (PORTD & 0xc3) | 0x3c;
	}
	else {
		PORTD = (PORTD & 0xc3) | 0;
	}*/
	
	if((PINB & 0xf) != 0){ //A key is pressed
		//Obtain row of key press
		key_row = (PINB & 0xf);

		//Swap input and output
		DDRB = 0xf;
		PORTB = 0xf;

		_delay_ms(100);
		//Obtain column of key press
		if((PINB & 0xf0) != 0){
			key_col = (PINB & 0xf0) >> 0x4;
		
			//Display key_row and key_col to test LEDs
			PORTD = ((PORTD & 0xc3) | (key_col << 0x4)) | (key_row << 0x2); // ToDo Difference between PIND and PORTD? Is this the right method?
		}
		else {
			PORTD = (PORTD & 0xc3) | 0;
		}

		//Parse input
		if (key_row == ROW1){
			if(key_col == COL1){
				input_char = 1;
			}
			else if(key_col == COL2){
				input_char = 2;
			}
			else if(key_col == COL3){
				input_char = 3;
			}
			else if(key_col == COL3){
				input_char = KEY_A;
			}
			else {
				input_char = KEY_UNKNOWN;
			}
		}
		else if (key_row == ROW2){
			if(key_col == COL1){
				input_char = 4;
			}
			else if(key_col == COL2){
				input_char = 5;
			}
			else if(key_col == COL3){
				input_char = 6;
			}
			else if(key_col == COL3){
				input_char = KEY_B;
			}
			else {
				input_char = KEY_UNKNOWN;
			}
		}
		else if (key_row == ROW3){
			if(key_col == COL1){
				input_char = 7;
			}
			else if(key_col == COL2){
				input_char = 8;
			}
			else if(key_col == COL3){
				input_char = 9;
			}
			else if(key_col == COL3){
				input_char = KEY_C;
			}
			else {
				input_char = KEY_UNKNOWN;
			}
		}	
		else if (key_row == ROW4){
			if(key_col == COL1){
				input_char = KEY_ASTERISK;
			}
			else if(key_col == COL2){
				input_char = 0;
			}
			else if(key_col == COL3){
				input_char = KEY_HASHTAG;
			}
			else if(key_col == COL3){
				input_char = KEY_D;
			}
			else {
				input_char = KEY_UNKNOWN;
			}
		}
		else {
			input_char = KEY_UNKNOWN;
		}

		//Trigger action
		//Numeric keys
		If(input_char <= 0x9){
			if (!is_entering){ //User just started entering frequency setting
				freq_temp = input_char;
				input_char_pos++;
				is_entering = TRUE;
			}
			else { //User is entering second or third digit go frequency setting
				freq_temp += input_char * pow(10, input_char_pos);
				input_char_pos++;
			}
		}

		//Swap input and output back
		DDRB = 0xf0;
		PORTB |= 0xf0;
	}
	else{
		PORTD = (PORTD & 0xc3) | 0;
	}
}


























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































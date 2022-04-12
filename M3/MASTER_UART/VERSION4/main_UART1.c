#define F_CPU		8000000UL // Clock speed

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define FALSE 0x0
#define TRUE 0x1

unsigned char received_char_int = '\0';
int RX_FLAG = FALSE;
char received_str[100];
int rx_counter = 0;

int TXEN = 0;
int UPDATE_LCD = TRUE;
int freq = 16000;

void UART_Init(){
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	//UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);   // Async-mode, 8 data bits, 1 stop bit, no parity bit
	UCSR0A = 0x0; // Clear the UASRT status register
	
	//Set Baud Rate
	UBRR0L = 51; // 9600 at 8 MHz
	UBRR0H = 0;
}

void TX_Char(char c){
	while(!(UCSR0A & (1<<UDRE0))); // Check transmission buffer empty flag UDRE0

	// Transmit data
	UDR0 = c;
}

char* RX_String(){
	int i = 0;
	//while()
}

void TX_String(char* str){
	char* ptr = str;
	
	while(*ptr != '\0'){
		TX_Char(*ptr);
		ptr++;
	}
	
	TX_Char(';');
}

char RX_Char(){
	while(!(UCSR0A & (1<<RXC0))); //Check receive completed flag RXC0

	// Receive data
	return UDR0;
}

int main(void){
	CLKPR = 1 << CLKPCE;
	CLKPR = 0;

	UART_Init();

	sei();

	/*while(1){
		char c1 = RX_Char();

		if (c1 != 0){
			TX_Char(c1);
		}
	}*/

	 while(1){
		 
	}
	
	//TX_String("Hello");
}

ISR(USART0_RX_vect){
	received_char_int = UDR0;
	if(received_char_int != ';'){
		received_str[rx_counter] = received_char_int;
		rx_counter++;
	}
	else{
		received_str[rx_counter] = ';';
		received_str[rx_counter + 1] = '\0';
		rx_counter = 0;
		
		// Parse instruction
		char inst[3];
		strncpy(inst, received_str, 2);
		int respond = (received_str[2] == ';') ? TRUE : FALSE;
		
		char str_to_send[100];
		str_to_send[0] = '\0';
		
		if(strcmp(inst, "TX") == 0){ //TX Command
			if(respond){
				strcat(str_to_send, "TX");
				char c[2];
				c[0] = TXEN ? '1' : '0';
				c[1] = '\0';
				strcat(str_to_send, c);
				TX_String(str_to_send);
			}
			else{
				char TXEN_to_set = received_str[2];
				TXEN = (TXEN_to_set == '1') ? TRUE : FALSE;
				UPDATE_LCD = TRUE;
				//TX_String("Set TXEN!");
			}
		}
		else if(strcmp(inst, "FA") == 0){
			if(respond){
				strcat(str_to_send, "FA");
				char c[10];
				c[0] = '\0';
				itoa(freq, c, 10);
				for (int i = 0; i < 6 - strlen(c); i++){
					strcat(str_to_send, "0");
				}
				strcat(str_to_send, c);
				strcat(str_to_send, "000");
				TX_String(str_to_send);
			}
			else{
				char freq_to_set[10];
				strncpy(freq_to_set, received_str + 2, 6);
				freq = atoi(freq_to_set);
			}
		}
		else if(strcmp(inst, "IF") == 0){
			strcat(str_to_send, "IF000");
			char c[10];
			c[0] = '\0';
			itoa(freq, c, 10);
			for (int i = 0; i < 6 - strlen(c); i++){
				strcat(str_to_send, "0");
			}
			strcat(str_to_send, c);
			strcat(str_to_send, "0000000000000000");
			TX_String(str_to_send);
		}
		else if(strcmp(inst, "SP") == 0){
			TX_String("Never Gonna Give You Up!");
		}
	}
}
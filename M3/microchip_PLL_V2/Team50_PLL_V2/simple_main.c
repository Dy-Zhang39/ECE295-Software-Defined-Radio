/*
 * PLL_Configuration_9.c
 *
 * Created: 4/10/2022 1:15:46 PM
 * Author : ECE295_Student
 */ 

#include <avr/io.h>
#include "i2c.h"
#include "si5351a.h"


int main(void)
{
    i2cInit();
	si5351aSetFrequency(14000000);
    while (1) 
    {
    }
}


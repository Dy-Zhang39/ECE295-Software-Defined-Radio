#include "si5351a.h"

int main(void)
{
	Si5351A_setup();

	si5351aSetFrequency(10000000);

	return 0;
}

#define CHANNEL_0 0


void si5351aSetFrequency(uint32_t frequency)
{
	uint32_t pllFreq;
	uint32_t xtalFreq = XTAL_FREQ;
	uint32_t l;
	float f;
	uint8_t mult;
	uint32_t num;
	uint32_t denom;
	uint32_t divider;

	divider = SI5351_PLL_VCO_MAX / frequency;// Calculate the division ratio. 900,000,000 is the maximum internal
									// PLL frequency: 900MHz
	if (divider % 2) divider--;		// Ensure an even integer division ratio

	pllFreq = divider * frequency;	// Calculate the pllFrequency: the divider * desired output frequency

	mult = pllFreq / xtalFreq;		// Determine the multiplier to get to the required pllFrequency
	l = pllFreq % xtalFreq;			// It has three parts:
	f = l;							// mult is an integer that must be in the range 15..90
	f *= 1048575;					// num and denom are the fractional parts, the numerator and denominator
	f /= xtalFreq;					// each is 20 bits (range 0..1048575)
	num = f;						// the actual multiplier is  mult + num / denom
	denom = 1048575;				// For simplicity we set the denominator to the maximum 1048575

									// Set up PLL A with the calculated multiplication ratio
	//setupPLL(SI5351_PLL_A, mult, num, denom);
	setupPLLInt(SI5351_PLL_A, 28) //testing number, hard coded
									// Set up MultiSynth divider 0, with the calculated divider.
									// The final R division stage can divide by a power of two, from 1..128.
									// reprented by constants SI_R_DIV1 to SI_R_DIV128 (see si5351a.h header file)
									// If you want to output frequencies below 1MHz, you have to use the
									// final R division stage
	//setupMultisynth(CHANNEL_0, SI5351_PLL_A, num, denom);
	setupMultisynthInt(CHANNEL_0, SI5351_PLL_A, 50)
									// Reset the PLL. This causes a glitch in the output. For small changes to
									// the parameters, you don't need to reset the PLL, and there is no glitch

	// Sets PLLB_rst and PLLA_rst to 1 in Reg 177
	write8(SI5351_REGISTER_177_PLL_RESET, 0xA0); // 0xA0 -> 10100000

									// Finally switch on the CLK0 output (0x4F)
									// and set the MultiSynth0 input to be PLL A
	write8(SI5351_REGISTER_16_CLK0_CONTROL, 0x4F | SI5351_PLL_A); // 01001111
}

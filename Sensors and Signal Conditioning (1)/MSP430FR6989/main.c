#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//set device configurations
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
//configure P1.2 as ADC input
    P1SEL0|=BIT2;
    P1SEL1|=BIT2;
//configure REF
    while(REFCTL0 & REFGENBUSY);             // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 + REFON;             // Enable internal 1.2V reference
//configure ADC
    ADC12CTL0 &= ~ADC12ENC;                   // Disable ADC12
    ADC12CTL0 |= ADC12SHT0_8 + ADC12ON;       // Set sample time
    ADC12CTL1 = ADC12SHP;                     // Enable sample timer
    ADC12MCTL0 = ADC12VRSEL_1 + ADC12INCH_2; // ADC input ch A2 for P1.2

    while(!(REFCTL0 & REFGENRDY));           // Wait for reference generator
                                                // to settle
    ADC12CTL0 |= ADC12ENC; //enables ADC
    ADC12CTL0|=ADC12SC;//starts conversion
	
	return 0;
}

#include <msp430.h> 


/**
 * main.c
 */
unsigned int temp[8];
int adcvalue;
int i = 0;

int linear(int in, int x_min, int x_max, int y_min, int y_max)
{
  return (in - x_min) * (y_max - y_min) / (x_max - x_min) + y_min;
}

int main(void)
{
       	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	/*set RGB outputs*/
	    P1DIR |= (BIT0+BIT1+BIT2);            //SETS P1.0,1,2 AS OUTPUT
	    P1OUT |= (BIT0+BIT1+BIT2);

	//initialize timer A0
	    TA0CTL = ( MC_1  + TASSEL_2);    //Set to up mode, run at SMCLK
	    TA0CCTL0 = CCIE;    // enable interrupts
	    TA0CCR0 = 1023;      //set PWM frequency to 1 KHz
	    TA0CCR1 = 0;        //initialize red Duty cycle to CCR1
	    TA0CCTL1 = CCIE;    // enable interrupts
	    TA0CCR2 = 0;        //initialize green PWM to CCR2
	    TA0CCTL2 = CCIE;    // enable interrupts
    //initialize timer A0
        TA1CTL = ( MC_1  + TASSEL_2);    //Set to up mode, run at SMCLK
        TA1CCTL0 = CCIE;    // enable interrupts
        TA1CCR0 = 1023;      //set PWM frequency to 1 KHz
        TA1CCR1 = 0;        //initialize blue Duty cycle to CCR1
        TA1CCTL1 = CCIE;    // enable interrupts
    //initialize ADC
	    ADC10CTL0 = ADC10SHT_0 + ADC10ON + ADC10IE; // 0 to 3.3 ref, ADC10ON
	    ADC10CTL1 = INCH_4 + CONSEQ_2;// input A4, repeat single channel, smclk
	    ADC10AE0|=BIT4;//set adc to P1.4

	    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

/*ISR FOR TIMERA0 CCR0*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & ADC10BUSY);
    if(TA0CCR1 != 0)          //if duty cycle is 0 keep output high
        P1OUT &= ~BIT0;

    if(TA0CCR2 != 0)        //if duty cycle is 0 keep output high
        P1OUT &= ~BIT1;

    if(1)//red -- low adc
        {
            if(adcvalue<20){
                TA0CCR1=1023;
            }
            else if(adcvalue<=400){
                TA0CCR1=linear(adcvalue, 20, 400, 1023, 0);
            }
            else if(adcvalue>350){
                TA0CCR1=0;
            }

        }
    if(1)//green -- mid adc
    {
        if(adcvalue<=200){
            TA0CCR2=0;
        }
        else if((adcvalue>250)&&(adcvalue<=500)){
            TA0CCR2 = linear(adcvalue, 250, 725, 1, 1023);
        }
        else if((adcvalue>500)&&(adcvalue<=725)){
            TA0CCR2 = linear(adcvalue, 500, 725, 1023, 1);
        }
        else if(adcvalue>725){
            TA0CCR2=0;
        }
    }


    if(1) //blue -- high adc
        {
            if(adcvalue>850){
                TA1CCR1=1023;//blue pwm to 100
            }
            else if((adcvalue>625)&&(adcvalue<=850)){
                TA1CCR1= linear(adcvalue, 625, 850, 0, 1023);
            }
            else if(adcvalue<=625){
                TA1CCR1=0;
            }
        }

    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    TA0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERA0 CCR1 and CCR2*/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void){

    switch (TA0IV){
    case 0x02:      //CCR1 (red)
        if(TA0CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT0;

        TA0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2 (green)
        if(TA0CCR2 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT1;

        TA0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}
/*ISR FOR TIMERA1 CCR0*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void){

    if(TA1CCR1 != 0)          //if duty cycle is 0 keep output high
        P1OUT &= ~BIT2;

    TA1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERA1 CCR1 and CCR2*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR (void){

        if(TA1CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT2;

        TA1CCTL1 &= ~BIT0;    //clears flags
}
// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
            temp[i] = ADC10MEM;
            i++;
            if (i==8){
                adcvalue = ((temp[0]+temp[1]+temp[2]+temp[3]+temp[4]+temp[5]+temp[6]+temp[7])>>3);
                i=0;
            }
}


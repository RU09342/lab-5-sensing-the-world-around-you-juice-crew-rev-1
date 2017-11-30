#include <msp430.h> 


/**
 * main.c
 */

int dT;
int ADCold;
int ADCnew;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    /*set RGB outputs*/
        P6DIR |= (BIT0+BIT1+BIT2);            //SETS P6.0,1,2 AS OUTPUT
        P6SEL0 &= ~(BIT0+BIT1+BIT2);        //SETS P6.0,1,2 MUX AS OUTPUT
        P6SEL1 &= ~(BIT0+BIT1+BIT2);        //SETS P6.0,1,2 MUX AS OUTPUT
        P6OUT &= ~(BIT0+BIT1+BIT2);

    //initialize timer A0
        TA0CTL = ( MC_1  + TASSEL_2  + ID_3);    //Set to up mode, run at SMCLK DIVIDE BY 8
        TA0CCTL0 = CCIE;    // enable interrupts
        TA0CCR0 = 1023;      //set PWM frequency to 1 KHz
        TA0CCR1 = 0;        //initialize red Duty cycle to CCR1
        TA0CCTL1 = CCIE;    // enable interrupts
        TA0CCR2 = 0;        //initialize green PWM to CCR2
        TA0CCTL2 = CCIE;    // enable interrupts

    //initialize timer A1
        TA1CTL = ( MC_1  + TASSEL_2  + ID_3);    //Set to up mode, run at SMCLK0 DIVIDE BY 8
        TA1CCTL0 = CCIE;    // enable interrupts
        TA1CCR0 = 1023;      //set PWM frequency to 2 Khz
        TA1CCR1 = 0;        //initialize blue PWM to TA1CCR1
        TA1CCTL1 = CCIE;    // enable interrupts

      __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
}

/*ISR FOR TIMERA0 CCR0*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    if(TA0CCR1 != 0)          //if duty cycle is 0 keep output high
            P1OUT &= ~BIT0;

        if(TA0CCR2 != 0)        //if duty cycle is 0 keep output high
            P1OUT &= ~BIT1;

        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & ADC10BUSY);
        dT=(ADC10MEM0-77);
        if(1)
        {
            if(dT>=0)
                {
                    TA0CCR1=(ADC10MEM0-77);
                    TA0CCR2=0;
                    TA1CCR1=0;
                }
            else if(dT<0)
                {
                    TA0CCR1=0;
                    TA0CCR2=0;
                    TA1CCR1=4*(77-ADC10MEM0);
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
            P6OUT |= BIT0;

        TA0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2 (green)
        if(TA0CCR2 != 1023)      //if duty cycle is 100%  keep output low
            P6OUT |= BIT1;

        TA0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}

/*ISR FOR TIMERA1 CCR0*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void){
    if(TA1CCR1 != 0)            //if duty cycle is 0 keep output high
        P6OUT &= ~BIT2;

    TA1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR (void){
    if(TA1CCR1 != 1023)      //if duty cycle is 100%  keep output low
        P6OUT |= BIT2;

    TA1CCTL1 &= ~BIT0;    //clears flags
}

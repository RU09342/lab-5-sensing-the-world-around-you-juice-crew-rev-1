#include <msp430.h> 


/**
 * main.c
 */
unsigned int temp[8];
int currentADC;
int i = 0;

int linear(int in, int x_min, int x_max, int y_min, int y_max)
{
  return ((in - x_min) * (y_max - y_min) / (x_max - x_min) + y_min);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

    /*set RGB outputs*/
        P1DIR|=(BIT0+BIT2+BIT3); //set p1 bit 0 for output
        P1SEL0&=~(BIT0+BIT2+BIT3); //set p1 bit 0 for I/O
        P1SEL1&=~(BIT0+BIT2+BIT3); //set p1 bit 0 for I/O

    //initialize timer B0
        TB0CTL = ( MC_1  + TBSSEL_2);    //Set to up mode, run at SMCLK
        TB0CCTL0 = CCIE;    // enable interrupts
        TB0CCR0 = 1023;      //set PWM frequency to 1 KHz
        TB0CCR1 = 0;        //initialize red Duty cycle to CCR1
        TB0CCTL1 = CCIE;    // enable interrupts
        TB0CCR2 = 0;        //initialize green PWM to CCR2
        TB0CCTL2 = CCIE;    // enable interrupts
    //initialize timer B0
        TB1CTL = ( MC_1  + TBSSEL_2);    //Set to up mode, run at SMCLK
        TB1CCTL0 = CCIE;    // enable interrupts
        TB1CCR0 = 1023;      //set PWM frequency to 1 KHz
        TB1CCR1 = 0;        //initialize blue Duty cycle to CCR1
        TB1CCTL1 = CCIE;    // enable interrupts
    //initialize ADC
        P1SEL1 |= BIT1;
        P1SEL0 |= BIT1;
        ADCCTL0 = ADCSHT_0 + ADCON; // 0 to 3.3 ref, ADC10ON
        ADCCTL1 = ADCCONSEQ_2+ ADCSHP+ ADCSSEL_2;//+ADCDIV_7;// input A1
        ADCCTL2 = ADCRES_1;
        ADCIE |= ADCIE0;
        ADCMCTL0 |= ADCINCH_1 + ADCSREF_0;

                                        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

/*ISR FOR TIMERB0 CCR0*/
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void){
    ADCCTL0 &= ~ADCENC;
   while (ADCCTL1 & ADCBUSY);
    if(TB0CCR1 != 0)          //if duty cycle is 0 keep output high
        P1OUT &= ~BIT0;

    if(TB0CCR2 != 0)        //if duty cycle is 0 keep output high
        P1OUT &= ~BIT2;

    if(1)//red
            {
                if(currentADC<45){
                    TB0CCR1=0;
                }
                else if((currentADC>45)&&(currentADC<=200)){
                    TB0CCR1=linear(currentADC, 45, 200, 0, 1023);
                }
                else if(currentADC>200){
                    TB0CCR1=1023;
                }

            }
    if(1)//green
        {
            if(currentADC<=15){
                TB0CCR2=0;
            }
            else if((currentADC>15)&&(currentADC<=30)){
                TB0CCR2 = linear(currentADC, 15, 25, 0 , 1023);
            }
            else if(currentADC>30&&currentADC<=50){
                TB0CCR2 = linear(currentADC, 30, 50, 0, 1023);
            }
            else if(currentADC>50){
                TB0CCR2=0;
            }
        }
        if(1) //blue
                  {
                      if(currentADC<=1){
                          TB1CCR1=1023;//blue pwm to 100
                      }
                      else if(currentADC>1&&currentADC<=20){
                          TB1CCR1= linear(currentADC, 1, 15, 1023, 0);
                      }
                      else if(currentADC>20){
                          TB1CCR1=0;
                  }
              }

    ADCCTL0 |= ADCENC + ADCSC;             // Sampling and conversion start
    TB0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB0 CCR1 and CCR2*/
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void){

    switch (TB0IV){
    case 0x02:      //CCR1 (red)
        if(TB0CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT0;

        TB0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2 (green)
        if(TB0CCR2 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT2;

        TB0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}
/*ISR FOR TIMERB1 CCR0*/
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR (void){

    if(TB1CCR1 != 0)          //if duty cycle is 0 keep output high
        P1OUT &= ~BIT3;

    TB1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1 and CCR2*/
#pragma vector=TIMER1_B1_VECTOR
__interrupt void Timer1_B1_ISR (void){

        if(TB1CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT3;

        TB1CCTL1 &= ~BIT0;    //clears flags
}
// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if((ADCIFG&BIT0)==BIT0)
            temp[i] = ADCMEM0;
            i++;
            if (i==8){
                currentADC = ((temp[0]+temp[1]+temp[2]+temp[3]+temp[4]+temp[5]+temp[6]+temp[7])>>3);
                i=0;
            }
}


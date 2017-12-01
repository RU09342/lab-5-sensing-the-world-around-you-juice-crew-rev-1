#include <msp430.h> 


/**
 * main.c
 */
unsigned int temp[8];
int tTemp;
int currentTemp;
int i = 0;

long linear(long in, long x_min, long x_max, long y_min, long y_max)
{
  return ((in - x_min) * (y_max - y_min) / (x_max - x_min) + y_min);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

    /*set RGB outputs*/
        P3DIR|=(BIT4+BIT5+BIT6); //set p1 bit 0 for output
        P3SEL0&=~(BIT4+BIT5+BIT6); //set p1 bit 0 for I/O
        P3SEL1&=~(BIT4+BIT5+BIT6); //set p1 bit 0 for I/O

    //initialize timer B0
        TA0CTL = ( MC_1  + TASSEL_2);    //Set to up mode, run at SMCLK
        TA0CCTL0 = CCIE;    // enable interrupts
        TA0CCR0 = 1023;      //set PWM frequency to 1 KHz
        TA0CCR1 = 0;        //initialize red Duty cycle to CCR1
        TA0CCTL1 = CCIE;    // enable interrupts
        TA0CCR2 = 0;        //initialize green PWM to CCR2
        TA0CCTL2 = CCIE;    // enable interrupts
    //initialize timer B0
        TA1CTL = ( MC_1  + TASSEL_2);    //Set to up mode, run at SMCLK
        TA1CCTL0 = CCIE;    // enable interrupts
        TA1CCR0 = 1023;      //set PWM frequency to 1 KHz
        TA1CCR1 = 0;        //initialize blue Duty cycle to CCR1
        TA1CCTL1 = CCIE;    // enable interrupts
    //initialize ADC12
        P1SEL1 |= BIT2;
        P1SEL0 |= BIT2;
        ADC12CTL0 = ADC12SHT0 + ADC12ON; // 0 to 3.3 ref, ADC1210ON
        ADC12CTL1 = ADC12CONSEQ_2+ ADC12SHP+ ADC12SSEL_2+ADC12DIV_7;// input A1
        ADC12CTL2 = ADC12RES_1;
        ADC12IER0 |= ADC12IE0;
        ADC12MCTL0 |= ADC12INCH_2;

        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

/*ISR FOR TIMERB0 CCR0*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    ADC12CTL0 &= ~ADC12ENC;
   while (ADC12CTL1 & ADC12BUSY);
    if(TA0CCR1 != 0)          //if duty cycle is 0 keep output high
        P3OUT &= ~BIT4;

    if(TA0CCR2 != 0)        //if duty cycle is 0 keep output high
        P3OUT &= ~BIT5;
    currentTemp=tTemp*0.32226;

    if(1)//green
    {
        if(currentTemp<=15){
            TA0CCR2=0;
        }
        else if((currentTemp>15)&&(currentTemp<=35)){
            TA0CCR2 = linear(currentTemp, 15, 35, 1, 1023);
        }
        else if(currentTemp>35&&currentTemp<=75){
            TA0CCR2 = linear(currentTemp, 35, 75, 1023, 0);
        }
        else if(currentTemp>75){
            TA0CCR2=0;
        }
    }

    if(1)//red
    {
        if(currentTemp<45){
            TA0CCR1=0;
        }
        else if(currentTemp>=45){
            TA0CCR1=linear(currentTemp, 45, 90, 1, 1023);
        }
        else if(currentTemp>90){
            TA0CCR1=1023;
        }

    }
    if(1) //blue
              {
                  if(currentTemp<=0){
                      TA1CCR1=1023;//blue pwm to 100
                  }
                  else if(currentTemp>0&&currentTemp<=45){
                      TA1CCR1= linear(currentTemp, 0, 45, 1023, 0);
                  }
                  else if(currentTemp>45){
                      TA1CCR1=0;
              }
          }

    ADC12CTL0 |= ADC12ENC + ADC12SC;             // Sampling and conversion start
    TA0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB0 CCR1 and CCR2*/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void){

    switch (TA0IV){
    case 0x02:      //CCR1 (red)
        if(TA0CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P3OUT |= BIT4;

        TA0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2 (green)
        if(TA0CCR2 != 1023)      //if duty cycle is 100%  keep output low
            P3OUT |= BIT5;

        TA0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}
/*ISR FOR TIMERB1 CCR0*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_B0_ISR (void){

    if(TA1CCR1 != 0)          //if duty cycle is 0 keep output high
        P3OUT &= ~BIT6;

    TA1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1 and CCR2*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_B1_ISR (void){

        if(TA1CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P3OUT |= BIT6;

        TA1CCTL1 &= ~BIT0;    //clears flags
}
// ADC12 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if((ADC12IFG0&BIT0)==BIT0)
            temp[i] = ADC12MEM0;
            i++;
            if (i==8){
                tTemp = ((temp[0]+temp[1]+temp[2]+temp[3]+temp[4]+temp[5]+temp[6]+temp[7])>>3);
                i=0;
            }
}


/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************

/**
 * main.c
 */
#include <driverlib/MSP430FR6989/driverlib.h>
#include <msp430.h>
#include "hal_LCD.h"
int sensor =1;
int s1;
int s2;
int s3;
int ADC_temp;
int ADCnew;
int ADCold;

void displayStatus(int sensor);


int main(void)
{
//set device configurations
     WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//configure P8.4 to sensor input
    P8SEL1|=(BIT4+BIT5+BIT6);
    P8SEL0|=(BIT4+BIT5+BIT6);

/*set RGB outputs*/
   P9DIR |= (BIT0+BIT1+BIT5);            //SETS P6.0,1,2 AS OUTPUT
   P9SEL0 &= ~(BIT0+BIT1+BIT5);        //SETS P6.0,1,2 MUX AS OUTPUT
   P9SEL1 &= ~(BIT0+BIT1+BIT5);        //SETS P6.0,1,2 MUX AS OUTPUT
   P9OUT &= ~(BIT0+BIT1+BIT5);

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

//initialize LCD
    Init_LCD();

/* Initialize ADC12_B */
    ADC12CTL0 &= ~ADC12ENC;                   // Disable ADC12
    ADC12CTL0 |= ADC12SHT0_1 + ADC12ON;       // Set sample time
    ADC12CTL1 |= ADC12SHP + ADC12CONSEQ1 + ADC12SSEL1;                     // Enable sample timer
    ADC12MCTL0 |=  ADC12INCH_5; // ADC input on channel 7 ADC12VRSEL_1 +

    //Timer set up
       TB0CTL= ( MC_1  + TBSSEL_1 + ID_3);             //up timer, smclk, div 8

       TB0CCTL0 = (CCIE);
       TB0CCR0= 4000;        // sets maximum timer value

       _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt
}
/*ISR FOR TIMERA0 CCR0*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    if(TA0CCR1 != 0)          //if duty cycle is 0 keep output high
        P9OUT &= ~BIT0;

    if(TA0CCR2 != 0)        //if duty cycle is 0 keep output high
        P9OUT &= ~BIT1;

    TA0CCR1=s1*4;
    TA0CCR2=s2*4;
    TA1CCR1=s3*4;

    TA0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERA0 CCR1 and CCR2*/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void){

    switch (TA0IV){
    case 0x02:      //CCR1 (red)
        if(TA0CCR1 != 1023)      //if duty cycle is 100%  keep output low
            P9OUT |= BIT0;

        TA0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2 (green)
        if(TA0CCR2 != 1023)      //if duty cycle is 100%  keep output low
            P9OUT |= BIT1;

        TA0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}

/*ISR FOR TIMERA1 CCR0*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void){
    if(TA1CCR1 != 0)            //if duty cycle is 0 keep output high
        P9OUT &= ~BIT5;

    TA1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR (void){
    if(TA1CCR1 != 1023)      //if duty cycle is 100%  keep output low
        P9OUT |= BIT5;

    TA1CCTL1 &= ~BIT0;    //clears flags
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void){
        ADC12CTL0 |= ADC12ENC + ADC12SC;                   // enable adc
            while(ADC12CTL1 & ADC12BUSY);
            __no_operation();
            displayStatus(sensor);
            if(sensor==1)//temperature
            {
                s1=ADC12MEM0;
                ADC_temp=ADC12MEM0/10;//converts voltage to temperature
                ADC12CTL0 &= ~ADC12ENC;
                ADC12CTL0 &=~ADC12SC;
                sensor+=1;
                ADC12MCTL0 = ADC12INCH_5;
            }
            else if(sensor==2)
            {
                s2=ADC12MEM0;
                ADC_temp=ADC12MEM0;
                ADC12CTL0 &= ~ADC12ENC;
                ADC12CTL0 &=~ADC12SC;
                sensor+=1;
                ADC12MCTL0 = ADC12INCH_7;
            }
            else if(sensor==3)
            {
                s3=ADC12MEM0;
                ADC12CTL0 &= ~ADC12ENC;
                ADC12CTL0 &=~ADC12SC;
                sensor=1;
                ADC12MCTL0 = ADC12INCH_6;
            }



    TB0CCTL0 &=~BIT0;    //clears flags
}

void displayStatus(int sensor)
{

            showChar('S',pos1);//left
            showChar(sensor+ '0',pos2);
            if (ADC_temp>=1000)
                showChar((ADC_temp/1000)%10 + '0',pos3);
            else
                showChar('0',pos3);
            if (ADC_temp>=100)
                    showChar((ADC_temp/100)%10 + '0',pos4);
            else
                            showChar('0',pos4);
            if (ADC_temp>=10)
                    showChar((ADC_temp/10)%10 + '0',pos5);
            else
                            showChar('0',pos5);
            if (ADC_temp>=1)
                    showChar((ADC_temp/1)%10 + '0',pos6);
            else
                            showChar('0',pos6);
}

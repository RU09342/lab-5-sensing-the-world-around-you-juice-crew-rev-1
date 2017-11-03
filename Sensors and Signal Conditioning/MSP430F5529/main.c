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

#include <msp430.h> 



unsigned int readTemp(void);

int main(void){

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    // uart set up
    P4SEL = BIT4+BIT5; // P3.4,5 = USCI_A0 TXD/RXD
    UCA1CTL1 |= UCSWRST; // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2; // SMCLK
    UCA1BR0 = 6; // 1MHz 9600 (see User's Guide)
    UCA1BR1 = 0; // 1MHz 9600
    UCA1MCTL = UCBRS_0 + UCBRF_13 + UCOS16; // Modln UCBRSx=0, UCBRFx=0,
    // over sampling
    UCA1CTL1 &= ~UCSWRST; // **Initialize USCI state machine**

    //Timer set up
    TA0CTL= ( MC_1  + TASSEL_1 + ID_3);             //up timer, smclk, div 8

    TA0CCTL0 = (CCIE);
    TA0CCR0= 4000;        // sets maximum timer value

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){

    while (!(UCA1IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
       UCA1TXBUF = readTemp();                  // TX -> RXed character

    TA0CCTL0 &=~BIT0;    //clears flags
}

unsigned int readTemp(void){

    // ADC Init
    REFCTL0 &= ~REFMSTR;
    P6SEL |= BIT2;
    ADC12CTL0 = ADC12SHT0_9+ADC12ON;
    ADC12CTL1 = ADC12SHP+ADC12CSTARTADD_2;
    ADC12MCTL2 = ADC12INCH_2;
    ADC12CTL0 |= ADC12SC+ADC12ENC;      // start conversion
    while(ADC12CTL1 & ADC12BUSY);
    __no_operation();

    return ADC12MEM2 & 0xFFF;
}


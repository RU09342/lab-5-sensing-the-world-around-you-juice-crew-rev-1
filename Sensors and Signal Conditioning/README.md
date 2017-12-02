# Overview
The code works by first initializing UART, the ADC, and timer and relying on the timer interrupt to both read and send the data through UART at a baud rate of 9600. 
## The ADC
The ADC is set to read on each timer interrupt. For the 5529 initializing by setting the ADC on, setting the sample hold time (ADC12CTL0), changing the starting address, and input channel. After this is complete, the ADC is enabled and starts conversion.
## Hardware
Please see the hardware readme file in the lab-5-sensing-the-world-around-you-juice-crew-rev-1.

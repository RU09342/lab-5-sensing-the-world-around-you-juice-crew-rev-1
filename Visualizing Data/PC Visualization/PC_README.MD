# Overview
This code works by communicating sensor data over UART with MATLAB using MATLAB's Serial function.

## Configuring UART
The UART on each board was configured to be at 9600 baud, which is based on the default communication baud rate on computers.
One limitation of UART was that the full resolution of the ADC was not used due to only 8 bits of the sensor data being sent at once through UART.
Because of this, the ADC value had to be shifted right twice which lost 2 bits of resolution.

The UART on the FR2311 used code provided by TI to compensate for a lack of hardware. The FR2311 does not have built in modulation so a baud rate of 9600 could only set through software.
##Reading Sensors
The ADC was set up to repeatedly read a single channel. This was done by reading based on a timer. The programs use the line

while(ADCCTL1 & ADCBUSY);

to wait until the ADC has finished converting to a digital signal. 

## MATLAB
MATLAB's Instrument Control Toolbox was used to read from the microprocessors. It was done by opening the toolbox, connecting to the corresponding COM port, then reading the data.
This is done by going to the receiving data section, setting the data type to binary, the data format to uint8. The data can then be read by clicking read. From here, right click then select export, and when the new window pops up, selection MATLAB Figure Window. The export button produces a graph of the UART values over time. The graphs below are what result from following this procedure. 

![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/Visualizing%20Data/fr2311%201.6V.jpg)

![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/Visualizing%20Data/g2553%201.6V.jpg)
## Hardware
To see the hardware around the device, please see the hardware readme.

## Processor selection choices
The FR2311 G2553 were the only 2 microprocessors used for this lab. This was because the only requirements were an ADC of 8 bit, and UART capability. For this reason, all 5 processors that we had stocked were available for use, but the cost of the FR2311 and G2553 are much lower than the other processors. Besides this, the other processors have no added benefit if the full ADC resolution cannot be used.

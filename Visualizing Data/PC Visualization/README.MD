#Overview
This code works by communicating sensor data over UART with MATLAB using MATLAB's Serial funciton.

##Configuring UART
The UART on each board was configured to be at 9600, which is based on the default communication baud rate on computers.
One limitation of UART was that the full resolution of the ADC was not used due to only 8 bits of the sensor data being sent at once through UART. 
Because of this, the ADC value had to be shifted right twice which lost 2 bits of resolution.

The UART on the FR2311 used code provided by TI to compensate for a lack of hardware. The FR2311 does not have built in modulation so a baudrate of 9600 could only set through software.
##Reading Sensors
The ADC was set up to repeatedly read a single channel. This was done by reading based on a timer. The programs uses the line

while(ADCCTL1 & ADCBUSY);

to wait until the adc has finished converting to a digitial signal. 

##MATLAB
MATLAB's Instrument Control Toolbox was used to read from the microprocessors. It was done by opening the toolbox, connection to the corrosponding COM port, then reading the data.
The data format for this was binary uint8. With both of these settings selected in MATLAB, a size of X was selected to get multiple samples for the plot. 
From this point, the data can be graphed by right clicking and selecting "create figure"********** 
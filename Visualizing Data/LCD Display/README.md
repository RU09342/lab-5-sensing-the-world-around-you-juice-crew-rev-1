#Overview
This program is designed to display out the temperature from 3 different sensors on the MSP430FR6989.
##Using the LCD Display
A driver library for the FR6989 and the files "hai_LCD.c" and "hai_LCD.h" were used to 
display out the sensor values. "hai_LCD.c" includes the function
showChar which displays out a character.
This allowed for the adc values to be displaced on the LCD. 
The way to display out a character in a specific spot is by setting pos within the funciton. 
Pos1 is the leftmost digit while pos6 is the rightmost. 
For example, the code below would display out the value of x as the rightmost digit on the LCD display.

showChar(x+'0', pos6)

oring x with 0 is nessacary or else all the segments of the position will be on.

##Using the ADC
Instead of using a multichannel repeating read from the ADC, the adc is instead enabled and disabled each time a sensor is read. 
While the ADC is disabled, the input to the ADC is changed to a different sensor. This is how the code read multiple sensors in repeat single channel mode.

###Processor Selection
The FR6989 was selected for this part because it was the only processor with an LCD screen.
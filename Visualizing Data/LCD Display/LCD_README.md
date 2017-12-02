# Overview
This program is designed to display out the ADC value from 3 different sensors on the MSP430FR6989.
## Using the LCD Display
A driver library for the FR6989 and the files "hai_LCD.c" and "hai_LCD.h" were used to display out the sensor values. "hai_LCD.c" includes the function showChar which displays out a character.
This allowed for the ADC values to be displaced on the LCD. 
The way to display out a character in a specific spot is by setting pos within the function. 
Pos1 is the leftmost digit while pos6 is the rightmost. 
For example, the code below would display out the value of x, where x is an int, as the rightmost digit on the LCD display.

showChar(x+'0', pos6)

ORing x with 0 is necessary to convert integers to characters.

One issue with this method is that integers with more than 1 digit cannot be displayed by the command above. Instead, the integer must be broken down into components of powers of 10. This is done by dividing the integer by a power of 10, then finding the remainder. For example, the code below checks if there is a 100’s place, and if there is, it divides the ADC_temp by 100, so the 100’s digit is now the ones digit, then find the remainder which isolates the ones digit. The same thing happens on the if statement below, except it checks for the 10’s digit instead of the 100’s.
            if (ADC_temp>=100)
                    showChar((ADC_temp/100)%10 + '0',pos4);
            if (ADC_temp>=10)
                    showChar((ADC_temp/10)%10 + '0',pos5); 
## Using the ADC
Instead of using a multichannel repeating read from the ADC, the ADC is instead enabled and disabled each time a sensor is read. 
While the ADC is disabled, the input to the ADC is changed to a different sensor. This is how the code read multiple sensors in repeat single channel mode.
## Hardware
To see the circuity around the hardware, please see the hardware readme file.

### Processor Selection
The FR6989 was selected for this part because it was the only processor with an LCD screen.

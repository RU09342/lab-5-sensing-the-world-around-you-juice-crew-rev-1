# Overview
This readme is to explan the hardware used in all part of this lab.
# Hardware
This lab focused around the ADC and reading values from different sensors. Some sensors in this lab changed resistance or current based on environmental factors. Because of this, external circuitry was used to convert each sensor output to a proportional voltage.
## Sensors
This lab used three sensors: a temperature sensor (LM35), a phototransistor (OP805SL), and a photoresistor (part number not listed). The following sections will describe the circuitry used for each of these sensors.
## Temperature Sensor
The LM35 needed no external circuitry. It can produce an output directly from the 5V onboard supply and ground. Because the LM35 requires 4V or more, the G2553 cannot be used with this directly unless there is an external power supply. 

![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/LM35.PNG)

## Phototransistor
The OP805SL differs slightly from other Phototransistors because it has a pin for its base. The reason why most phototransistors have 2 pins is that the purpose changes with respect to light rather than input current.

![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/Phototransistor.PNG)

A 100kohm resistor was selected because it has a very low current, while also providing adequte swing. The resistor was also selected because it the output was about 1.3V when in a medium light. This was found through trial and error testing.

When measuring the circuit output when connected to 3.3 volts, the adc measurement yielded a voltage range of 0.08V to 3.3V.
### Photoresistor
The photoresistor has no part number. To check the device characteristics the range of the photoresistor was measured. The resistance had a range between 300Ohms and 1.3Mohms. Because the device had no datasheet, it was assumed that the photo resistor had a maximum current around 1 mA, to be on the safe side of things. The circuit used a 11kohm resistor in series with the photo resistor to generate the output. 

![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/Photoresistor.PNG)

The lower current also reduced the power needed to for the sensor to run.

## LED
A common anode RGB LED was used for lab. Although it is less intuitive, this LED had better color accuracy compared to other common cathode LEDs on hand. This LED setup was used for all portions of this lab.

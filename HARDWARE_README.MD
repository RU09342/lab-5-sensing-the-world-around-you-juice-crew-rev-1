#Hardware
This lab focused around the ADC and reading values from different sensors. Some sensors in this lab changed resistance or current based on environmental factors. Because of this, external circuitry was used to convert each sensor output to a proportional voltage.
##Sensors
This lab used three sensors: a temperature sensor (LM35), a phototransistor (OP805SL), and a photoresistor (part number not listed). The following sections will describe the circuitry used for each of these sensors.
##Temperature Sensor
The LM35 needed no external circuitry. It can produce an output directly from the 5V onboard supply and ground. Because the LM35 requires 4V or more, the G2553 cannot be used with this directly unless there is an external power supply. 
![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/LM35.PNG
##Phototransistor)
The OP805SL differs slightly from other Phototransistors because it has a pin for its base. The reason why most phototransistors have 2 pins is that the purpose changes with respect to light rather than input current.
![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/Phototransistor.PNG)
###Photoresistor
The photoresistor has no part number. To check the device characteristics the range of the photoresistor was measured. The resistance had a range between 300Ohms and 1.3Mohms. Because the device had no datasheet, it was assumed that the photo resistor had a maximum current around 5 mA, to be on the safe side of things. The circuit used a 11kohm resistor in series with the photo resistor to generate the output. 
![Alt test](https://github.com/RU09342/lab-5-sensing-the-world-around-you-juice-crew-rev-1/blob/master/readme%20images/Photoresistor.PNG)
##LED
A common anode RGB LED was used for lab. Although it is less intuitive, this LED had better color accuracy compared to other common cathode LEDs on hand.

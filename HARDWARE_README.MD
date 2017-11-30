##Hardware
###Reading from the photoresistor
The photo resistor changes resistance based on the level of light it takes in. It was measure to have a minimum voltage of 400 Ohms?
The microprocessors ADC can be used to obtain a value from this, but it requres hardware surrounding the photoresistor. 
A resistor divider network was used to get a voltage proportional to the photoresistors resistance. The schematic for this can be seen below

Insert Photo here

Since the photoresistor is placed between the microprocessor and ground, the voltage on the microprocessors input pin is Vin*(Pr/(Pr+240?))
This is assuming that the input current to the microprocessor is neglagable.
A 240? Ohm resistor was used to limit the current throught the photoresistor. It was assumed that the photoresistor a current limit slightly above 1mA.****

###Reading from 

###Reading from the temperature sensor
The temperature sensor used was the LM35. This directly outputs a voltage based on the temperature.
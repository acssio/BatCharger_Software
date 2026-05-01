# BatCharger_Software
Software of the battery charger for the Circuit Design Laboratory

## ADC
For the adc conversion, we have a 12 bits ADC. With means that when the input = Vref (3300 mV), ADC value should be 4096.
To read the voltage input in mV, we need to do a little conversion. (ReadValue*Vref)/2^12

## LEDs
There is 3 leds on the circuit board. 
- LD1 :	(ON) Battery is charging
		(BLINK) Battery is discharging
		
- LD2 :	(STATIC) Battery Charged / Stocked

- LD3 : (BLINK) Voltage Error
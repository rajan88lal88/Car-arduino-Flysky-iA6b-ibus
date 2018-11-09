# Car-arduino-Flysky-iA6b-ibus

This code decodes the flysky receiver iA6b ibus protocol on arduino to make a car. The car uses L298 or L293 motor driver and flysky IA6b receiver.( I havent tested other ibus receivers).
The receiver's ibus output is connected to the Serial Rx pin.
The code only uses channel channel 1,2(pitch and roll) and 5(Aux channel to set maximum speed of car). You can change it accordingly as per your requirements.

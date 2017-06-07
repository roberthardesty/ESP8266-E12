#include <stdio.h>
#include "irslinger.h"

int main(int argc, char *argv[])
{
	uint32_t outPin = 23;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	int repeatCodes[] = {9110, 2167, 610};       // the LED will turn on for half the cycle time, and off the other half
	int repeatArray[argv[0] * 3];
	int j;
	for(j = 0; j < argv[0] * 3; j++){
			int k;
		for(k = 0;k < 3;k++){
			repeatArray[j + k] = repeatCodes[k];
		}
	}

	int codes[67 + (argv[0] * 3)] = {9002, 4434, 608, 516, 608, 517, 607, 520, 606, 1636, 612, 1634, 615, 1634, 611, 516, 606
	, 519, 606, 1639, 611, 1636, 609, 1638, 608, 517, 608, 519, 605, 520, 607, 1640, 606, 1638, 609, 517, 608
	, 1636, 614, 1636, 610, 1636, 610, 515, 610, 519, 608, 513, 610, 515, 610, 1635, 616, 515, 605, 521, 606
	, 518, 607, 1637, 610, 1637, 608, 1637, 610, 1637, 610, 39926, 9116, 2167, 611};


	int i;
	for(i =0; i < argv[0] * 3; i++)
		codes[67 + i] = repeatArray[i];

	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));

	return result;
}

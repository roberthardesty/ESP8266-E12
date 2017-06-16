#include <stdio.h>
#include "irslinger.h"
#include "irCodes.h"

int main(int argc, char *argv[])
{
    uint32_t outPin = 23;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
    int repeatCodes[] = {9110, 2167, 610};     
 
    char commandType = (char)argv[0];    
    int repeatCodesCount = ((int)argv[1]) * 3;
    int repeatArray[repeatCodesCount];   
    
    //load up the desired amount of repeat command codes into an Array
    int j;
	for(j = 0; j < repeatCodesCount; j+=3){
        repeatArray[j] = repeatCodes[j % 3];
	}
    printf(commandType);
    //load desired command from header file
    int commandArray[71];
    if(commandType == "u")
         memcpy(commandArray, SANYO_VOLUME_UP, 71 * sizeof(int));//   commandArray = SANYO_VOLUME_UP;
    else if(commandType == "d")
        memcpy(commandArray, SANYO_VOLUME_DOWN, 71 * sizeof(int));//   commandArray = SANYO_VOLUME_DOWN;
    else if(commandType == "p")
         memcpy(commandArray, SANYO_POWER, 71 * sizeof(int)); //commandArray = SANYO_POWER;
    else if(commandType == "m")
         memcpy(commandArray, SANYO_MUTE, 71 * sizeof(int));//commandArray = SANYO_MUTE;
    else if(commandType == "s")
         memcpy(commandArray, SANYO_SOURCE, 71 * sizeof(int)); //  commandArray = SANYO_SOURCE;
    else{
        printf("Invalid Agrument Dude: %s\n" commandType);
        return 1;
    }
    
    //copy command code array and subsequent repeat code array to one array
    int myIRCode[71 + (repeatCodesCount)];
    memcpy(myIRCode, command, 71 * sizeof(command[0])); // copy 4 floats from x to total[0]...total[3]
    memcpy(myIRCode + 71, repeatArray, repeatCodesCount * sizeof(repeatArray[0])); // copy 4 floats from y to total[4]...total[7]
    //'sling' it
	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		myIRCode,
		sizeof(myIRCode) / sizeof(int));

	return result;
}

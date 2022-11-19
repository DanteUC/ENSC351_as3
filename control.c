#include "control.h"
#include "buttons.h"
#include "joystick.h"
#include "audioMixer.h"
#include "matrix.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void control_joyStickControl(){
    while(1){
        int direction = joystick_getDirection();
        int currentVolume;
        switch (direction)
        {
        case 1:
            //volume up
            currentVolume = AudioMixer_getVolume();
            AudioMixer_setVolume(currentVolume+5);
            matrix_displayInteger(currentVolume+5);
            Utils_sleepForMs(1000);

            while(joystick_getDirection() == 1){
                currentVolume = AudioMixer_getVolume();
                AudioMixer_setVolume(currentVolume+5);
                matrix_displayInteger(currentVolume+5);
                Utils_sleepForMs(500);
            }

            break;
        case 2:
            //volume down
            currentVolume = AudioMixer_getVolume();
            AudioMixer_setVolume(currentVolume-5);
            matrix_displayInteger(currentVolume-5);
            Utils_sleepForMs(1000);

            while(joystick_getDirection() == 2){
                currentVolume = AudioMixer_getVolume();
                AudioMixer_setVolume(currentVolume-5);
                matrix_displayInteger(currentVolume-5);
                Utils_sleepForMs(500);
            }
            break;
        case 3:
            //tempo up
            
            printf("tempo up\n");
            Utils_sleepForMs(1000);
            while(joystick_getDirection() == 3){
                printf("tempo up\n");
                Utils_sleepForMs(500);
            }
            break;
        case 4:
            //tempo down
            
            printf("tempo down\n");
            Utils_sleepForMs(1000);
            while(joystick_getDirection() == 4){
                printf("tempo down\n");
                Utils_sleepForMs(500);
            }
            break;
        default:
            break;
        }
    }
}
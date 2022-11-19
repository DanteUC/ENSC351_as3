#include "audioMixer.h"
#include "matrix.h"
#include "joystick.h"
#include "buttons.h"
#include "utils.h"
#include "control.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define BASEDRUM_FILE "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"

int main()
{
    printf("Starting beatbox...\n");

    matrix_initializeMatrix();
    //AudioMixer_init();
    buttons_initializeButtons();

    matrix_displayMode(1);
    //testEdgeTrigger();
    
    control_joyStickControl();

    

    // Load wave file we want to play:
	// wavedata_t sampleFile;
	// AudioMixer_readWaveFileIntoMemory(BASEDRUM_FILE, &sampleFile);


    // AudioMixer_cleanup();

    // while(1){
        
    // }
   
    return 0;
}
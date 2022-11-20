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
#include <ctype.h>

//#define BASEDRUM_FILE "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"

int main()
{
    printf("Starting beatbox...\n");

    matrix_initializeMatrix();
    //AudioMixer_init();
    buttons_initializeButtons();
    control_startMatrix();
    control_startPollingButtons();
    control_startPollingJoystick();
    // while(1){
    //     buttons_pollButtons();
    // }
    

    printf("Enter 'Q' to quit.\n");
    while (true) {
        // Quit?
        if (toupper(getchar()) == 'Q') {
            break;
        }
    }


    // Load wave file we want to play:
	// wavedata_t sampleFile;
	// AudioMixer_readWaveFileIntoMemory(BASEDRUM_FILE, &sampleFile);

   
    // AudioMixer_cleanup();

    control_stopPollingButtons();
    control_stopPollingJoystick();
    control_stopMatrix();
    return 0;
}
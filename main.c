#include "audioMixer.h"
#include "matrix.h"
#include "joystick.h"
#include "beatGenerator.h"
#include "buttons.h"
#include "utils.h"
#include "control.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>


int main()
{
    printf("Starting beatbox...\n");

    matrix_initializeMatrix();
    AudioMixer_init();
    beatGenerator_init();
    buttons_initializeButtons();
    control_startThreads();

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
    control_UICleanup();
    printf("stopped UI threads\n");
    beatGenerator_cleanup();
    printf("stopped beat generator\n");
    AudioMixer_cleanup();
    printf("stopped audiomixer\n");

    
    return 0;
}
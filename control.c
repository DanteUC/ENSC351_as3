#include "control.h"
#include "buttons.h"
#include "joystick.h"
#include "audioMixer.h"
#include "beatGenerator.h"
#include "matrix.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_t printid;
static pthread_t buttonid;
static pthread_t joystickid;
static pthread_t matrixid;
bool isExit = false;
static bool matrixValueChanged = false;
static int currentMode = 0;


int control_getMode()
{
    return currentMode;
}
static void resetMatrix()
{
    if(!matrixValueChanged){
        matrix_displayMode(currentMode);
    }else{
        matrixValueChanged = false;
    }
}

void control_updateMode()
{
    if(currentMode==2){
        currentMode = 0;
        matrix_displayMode(currentMode);
        //also update the beat mode in audio
    }else{
        currentMode++;
        matrix_displayMode(currentMode);
        //update audio mode here
    }
    
}

static void control_pollJoystick()
{
    
        int direction = joystick_getDirection();
        int currentVolume=0;
        int currentBPM=0;
        switch (direction)
        {
        case 1:
            //volume up
            currentVolume = AudioMixer_getVolume();
            AudioMixer_setVolume(currentVolume+5);
            matrix_displayInteger(currentVolume+5);
            matrixValueChanged = true;
            Utils_sleepForMs(1000);

            while(joystick_getDirection() == 1){
                currentVolume = AudioMixer_getVolume();
                AudioMixer_setVolume(currentVolume+5);
                matrix_displayInteger(currentVolume+5);
                matrixValueChanged = true;
                Utils_sleepForMs(500);
            }

            break;
        case 2:
            //volume down
            currentVolume = AudioMixer_getVolume();
            AudioMixer_setVolume(currentVolume-5);
            matrix_displayInteger(currentVolume-5);
            matrixValueChanged = true;
            Utils_sleepForMs(1000);

            while(joystick_getDirection() == 2){
                currentVolume = AudioMixer_getVolume();
                AudioMixer_setVolume(currentVolume-5);
                matrix_displayInteger(currentVolume-5);
                matrixValueChanged = true;
                Utils_sleepForMs(500);
            }
            break;

        case 3:
            //tempo up
            currentBPM = beatGenerator_getbpm();
            beatGenerator_setbpm(currentBPM+5);
            matrixValueChanged = true;
            matrix_displayInteger(beatGenerator_getbpm());
            Utils_sleepForMs(1000);
            while(joystick_getDirection() == 3){
                currentBPM = beatGenerator_getbpm();
                beatGenerator_setbpm(currentBPM+5);
                matrix_displayInteger(beatGenerator_getbpm());
                matrixValueChanged = true;
                Utils_sleepForMs(500);
            }
            break;

        case 4:
            //tempo down
            currentBPM = beatGenerator_getbpm();
            beatGenerator_setbpm(currentBPM-5);
            matrixValueChanged = true;
            matrix_displayInteger(beatGenerator_getbpm());
            Utils_sleepForMs(1000);
            while(joystick_getDirection() == 4){
                currentBPM = beatGenerator_getbpm();
                beatGenerator_setbpm(currentBPM-5);
                matrix_displayInteger(beatGenerator_getbpm());
                matrixValueChanged = true;
                Utils_sleepForMs(500);
            }
            break;

        default:
            break;
        }
}

static void *control_print(void * arg){
    while(!isExit){
        printf("mode:%i\n",control_getMode());
        printf("tempo:%i\n beats per minute\n", beatGenerator_getbpm());
        printf("volume:%i\n", AudioMixer_getVolume());
        //time between each playback buffer??
        printf("8th note interval:%lld\n",beatGenerator_calculateHalfBeatInMs());
        Utils_sleepForMs(1000);
    }
    
    return NULL;
}

static void *control_buttonsControl(void* arg)
{
    while(!isExit){
        buttons_pollButtons();
    }
    return NULL;
}

static void *control_matrix(void* arg)
{
    while(!isExit){
        resetMatrix();
        Utils_sleepForMs(2000);
    }
    return NULL;
}
static void *control_joystickControl(void* arg)
{
    while(!isExit){
        control_pollJoystick();
    }
    return NULL;
}
void control_startPollingButtons(void)
{
    pthread_attr_t buttonattr;
    pthread_attr_init (&buttonattr);
    pthread_create(&buttonid, &buttonattr, control_buttonsControl, NULL);
}

void control_startPollingJoystick(void)
{
    pthread_attr_t joystickattr;
    pthread_attr_init (&joystickattr);
    pthread_create(&joystickid, &joystickattr, control_joystickControl, NULL);
}

void control_startMatrix()
{
    pthread_attr_t matrixattr;
    pthread_attr_init (&matrixattr);
    pthread_create(&matrixid, &matrixattr, control_matrix, NULL);
}

void control_startPrint(void){
    pthread_attr_t printidattr;
    pthread_attr_init (&printidattr);
    pthread_create(&buttonid, &printidattr, control_print, NULL);
}
void control_stopPollingButtons(void)
{
    isExit = true;
    pthread_join(buttonid, NULL);
}

void control_stopPollingJoystick(void)
{
    isExit = true;
    pthread_join(joystickid, NULL);
}
void control_stopMatrix()
{
    isExit = true;
    pthread_join(matrixid, NULL);
}

void control_stopPrint(){
    isExit = true;
    pthread_join(printid, NULL);
}

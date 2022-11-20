#include "beatGenerator.h"
#include "audioMixer.h"
#include "control.h"
#include "utils.h"
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

static int bpm = 120;

void* beatGenerator_thread(void* _arg);
static pthread_t beatGenThreadId;
//static pthread_mutex_t beatGenMutex = PTHREAD_MUTEX_INITIALIZER;

static _Bool stopping = false;

void beatGenerator_init()
{
    bpm = 120;
    pthread_create(&beatGenThreadId, NULL, beatGenerator_thread, NULL);
}


void beatGenerator_cleanup()
{
    stopping = true;
    pthread_join(beatGenThreadId, NULL);
}


long long beatGenerator_calculateHalfBeatInMs()
{   
    return (60/(double)(bpm/(double)2))*1000;
}

static void beatGenerator_playRockBeat()
{
    AudioMixer_playBaseDrum();
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    AudioMixer_playSnare();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playBaseDrum();
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    AudioMixer_playSnare();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
}

static void beatGenerator_playCustomBeat()
{
    AudioMixer_playBaseDrum();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playBaseDrum();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playBaseDrum();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playBaseDrum();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    AudioMixer_playHiHat();
    Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    
}

int beatGenerator_getbpm()
{
    return bpm;
}

void beatGenerator_setbpm(int setBPM)
{
    if(setBPM > 300 || setBPM < 40){
        //do nothing
    }else{
        bpm = setBPM;
    }
}

// void sleepForMs(long long delayInMs)
// {
//     const long long NS_PER_MS = 1000 * 1000;
//     const long long NS_PER_SECOND = 1000000000;
    
//     long long delayNs = delayInMs * NS_PER_MS;
//     int seconds = delayNs / NS_PER_SECOND;
//     int nanoseconds = delayNs % NS_PER_SECOND;
    
//     struct timespec reqDelay = {seconds, nanoseconds};
//     nanosleep(&reqDelay, (struct timespec *) NULL);
// }


void* beatGenerator_thread(void* _arg)
{
    int mode = 0;
    while(!stopping){
        mode = control_getMode();

        if(mode == 1){
            beatGenerator_playRockBeat();
        }else if (mode == 2) {
            beatGenerator_playCustomBeat();
        } else {
            //do nothing
        }
        Utils_sleepForMs(beatGenerator_calculateHalfBeatInMs());
    }

    return NULL;
}
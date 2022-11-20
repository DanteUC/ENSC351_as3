#include "beatGenerator.h"
#include "audioMixer.h"
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

int bpm = 120;

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


long long calculateHalfBeat_inMs()
{   
    return (60/(double)(bpm/(double)2))*1000;
}

void playRockBeat()
{
    AudioMixer_playBaseDrum();
    AudioMixer_playHiHat();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
    AudioMixer_playSnare();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playBaseDrum();
    AudioMixer_playHiHat();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
    AudioMixer_playSnare();
    sleepForMs(calculateHalfBeat_inMs());
    AudioMixer_playHiHat();
}


int get_bpm()
{
    return bpm;
}

void set_bpm(int setBPM)
{
    if(setBPM > 300 || setBPM < 40){
        //do nothing
    }else{
        bpm = setBPM;
    }
}

void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}


void* beatGenerator_thread(void* _arg)
{
    while(!stopping){
        //if(mode == 1){

        //} else if (mode == 2) {
            printf("playing rock beat...\n");
            playRockBeat();
        //} else {

        //}

        sleepForMs(calculateHalfBeat_inMs());
        
    }

    return NULL;
}
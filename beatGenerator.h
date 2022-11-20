#ifndef BEAT_GENERATOR_H
#define BEAT_GENERATOR_H

void beatGenerator_init();
void beatGenerator_cleanup();

void playRockBeat();

long long calculateHalfBeat_inMs();

int get_bpm();
void set_bpm(int setBPM);

void sleepForMs(long long delayInMs);

#endif
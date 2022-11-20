#ifndef BEAT_GENERATOR_H
#define BEAT_GENERATOR_H

void beatGenerator_init();
void beatGenerator_cleanup();

int beatGenerator_getbpm();

void beatGenerator_setbpm(int setBPM);
long long beatGenerator_calculateHalfBeatInMs();

#endif
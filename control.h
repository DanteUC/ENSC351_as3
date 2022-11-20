//this module handles the high level controls, abstracting away joystick and button as 
//as well as initialization and cleanup of thread

#ifndef CONTROL_H
#define CONTROL_H
#include <stdbool.h>
//start the threads
void control_startThreads();
//cleanup memory and join threads
void control_stopThreads();

//updates the current application mode by incrementing one or setting back to 0
void control_updateMode();
//fet
int control_getMode();

#endif
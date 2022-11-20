//this module handles the high level controls, abstracting away joystick and button as 
//as well as initialization and cleanup of thread

#ifndef CONTROL_H
#define CONTROL_H
#include <stdbool.h>
//start the threads
void control_startPollingButtons(void);
void control_startPollingJoystick(void);
void control_startMatrix(void);
void control_startPrint(void);

//cleanup memory and join threads
void control_stopPollingButtons(void);
void control_stopPollingJoystick(void);
void control_stopMatrix(void);
void control_stopPrint(void);

//updates the current application mode, 
void control_updateMode();
int control_getMode();

#endif
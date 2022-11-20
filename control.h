//this module handles the high level controls, abstracting away joystick and button as 
//as well as initialization and cleanup of thread

#ifndef CONTROL_H
#define CONTROL_H


//start the threads
void control_startPollingButtons(void);
void control_startPollingJoystick();

//cleanup memory and join threads
void control_stopPollingButtons(void);
void control_stopPollingJoystick(void);

#endif
//this module handles the high level controls, abstracting away joystick and button as 
//as well as initialization and cleanup of thread

#ifndef CONTROL_H
#define CONTROL_H

//creates a thread which polls joystick every 100ms, if it's pressed, update the correct value then if the joystick
//is held repeats that action every 1/2 second
void control_joyStickControl();

//creates a thread to control buttons 
void control_buttonsControl();
void control_cleanup();
#endif
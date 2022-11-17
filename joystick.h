//This package allows the user to capture the raw values from the joystick
//and scales the X and Y values between -1.0 and 1.0
//written by Dante Umpherville-Choy 

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define PATH_Y = /sys/bus/iio/devices/iio:device0/in_voltage2_raw
#define PATH_X = /sys/bus/iio/devices/iio:device0/in_voltage3_raw
#define MIDPOINT 2048

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//returns 1 for up, 2 for down, 3 for right, 4 for left and 0 for no value
//will prioritize up or down over left and right
//i.e if up or down value is at least half of max, it will return that value
int getDirection();
void writeToMatrix();
void testJoystick();
#endif
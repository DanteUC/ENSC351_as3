//This package creates a simple interface to use the buttons as input for the Air-Guitar-Hero project

#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTON_1 "/sys/class/gpio/gpio47"
#define BUTTON_2 "/sys/class/gpio/gpio46"
#define BUTTON_3 "/sys/class/gpio/gpio27"
#define BUTTON_4 "/sys/class/gpio/gpio65"
#define VALUE_PATH "/value"
#define DIRECTION_PATH "/direction"
#define ACTIVE_LOW_PATH "/active_low"
#define EDGE "/edge"

//This function initializes the buttons as gpio and sets them to input
void ButtonArray_initializeButtons();

//user must pass in an array of 5 button values to be edited to contain the values of the buttons
void ButtonArray_getButtonValues(int *values);

void ButtonArray_test();

// Convert button values from file to note value schema defined in note queue
int ButtonArray_buttonValuesToNote(int *values);

#endif
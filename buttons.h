//This package creates a simple interface to use the buttons as input for the Air-Guitar-Hero project

#ifndef BUTTONS_H
#define BUTTONS_H


//This function initializes the buttons as gpio and sets them to input
void buttons_initializeButtons();

void buttons_pollButtons();
#endif
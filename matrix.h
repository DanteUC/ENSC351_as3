//This module allows the user to display integer values between 00 and 99 
//and decimal values between 0.0 to 9.9 with up to one tenth degree of accuracy
//written by Dante Umpherville-Choy and Divyam ***** on 10/30/2022
#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <time.h>

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define REG_ADDR 0x00
#define NUM_BYTES 16
#define BUS_ADDR 0x70
#define MATRIX_LENGTH 8


void initializeMatrix();

//takes an int from 1-4
void displayMode(int mode);

//displays a positive value between 0 and 99
//any numbers above 99 will display 99, numbers below 0 will display 0
void displayInteger(int value);

//Displays a decimal value on the matrix to one decimal place. 
//Values above 9.9 will display 9.9, values below 0 will display 0
void displayDouble(double value);




#endif 
#include "buttons.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <fcntl.h> 
#include <errno.h>
#include <unistd.h>

#define BUTTON_ARRAY_SIZE 4
#define RISING_EDGE "rising"

char* ButtonArray[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

void ButtonArray_getButtonValues(int *values)
{
    for(int i = 0; i< BUTTON_ARRAY_SIZE; i++){
        values[i] = Utils_readIntFromFile(Utils_concat(ButtonArray[i], VALUE_PATH));
    }
}


void* buttonsThread(void* arg)
{

}
static _Bool stopping = false;
static pthread_t buttonsThreadid;
static pthread_mutex_t audioMutex = PTHREAD_MUTEX_INITIALIZER;


//Dr. Brian's code for edge trigger GPIO
//adjusted for multiple files
static int waitForGpioEdge(const char* fileNameForGpioValue) 
{
	// If you want to wait for input on multiple file, you could change this function
	// to take an array of names, and then loop throug each, adding it to the 
	// epoll instance (accessed via epollfd) by calling epoll_ctl() numerous times.

	// create an epoll instance
	// .. kernel ignores size argument; must be > 0
	int epollfd = epoll_create(1);
	if (epollfd == -1) {
		fprintf(stderr, "ERROR: epoll_create() returned (%d) = %s\n", epollfd, strerror(errno));
		return -1;
	}

	// open GPIO value file:
	int fdLeft = open(fileNameForGpioValue, O_RDONLY | O_NONBLOCK);
	if (fdLeft == -1) {
		fprintf(stderr, "ERROR: unable to open() GPIO value file (%d) = %s\n", fdLeft, strerror(errno));
		return -1;
	}

	// configure epoll to wait for events: read operation | edge triggered | urgent data
	struct epoll_event epollStruct;
	epollStruct.events = EPOLLIN | EPOLLET | EPOLLPRI;
	epollStruct.data.fd = fdLeft;

	// register file descriptor on the epoll instance, see: man epoll_ctl
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fdLeft, &epollStruct) == -1) {
		fprintf(stderr, "ERROR: epoll_ctl() failed on add control interface (%d) = %s\n", fdLeft, strerror(errno));
		return -1;
	}

	// ignore first trigger
	for (int i = 0; i < 2; i++) {
		int waitRet = epoll_wait(
				epollfd, 
				&epollStruct, 
				1,                // maximum # events
				-1);              // timeout in ms, -1 = wait indefinite; 0 = returne immediately

		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			close(fdLeft);
			close(epollfd);
			return -1;
		}
	}

	// cleanup epoll instance:
	close(fdLeft);
	close(epollfd);
	return 0;
}


void ButtonArray_initializeButtons()
{
    //set each config
    Utils_runCommand("config-pin P8.15 gpio");
    Utils_runCommand("config-pin P8.16 gpio");
    Utils_runCommand("config-pin P8.17 gpio");
    Utils_runCommand("config-pin P8.18 gpio");
    
    for(int i=0; i < BUTTON_ARRAY_SIZE; i++){
        char *directionPath = Utils_concat(ButtonArray[i], DIRECTION_PATH);
        Utils_writeToFile(directionPath, "in");
        char *active_low = Utils_concat(ButtonArray[i], ACTIVE_LOW_PATH);
        Utils_writeToFile(active_low, "0");
        char *edgePath = Utils_concat(ButtonArray[i], EDGE);
        Utils_writeToFile(edgePath, RISING_EDGE);
    }
}

void ButtonArray_test()
{
    while(true){
        int values[BUTTON_ARRAY_SIZE];
        ButtonArray_getButtonValues(values);
        for(int i=0; i< BUTTON_ARRAY_SIZE; i++){
            printf("Button %i: %i\n", i, values[i]);
        }
        ButtonArray_buttonValuesToNote(values);
        Utils_sleepForMs(100);
    }
}

int ButtonArray_buttonValuesToNote(int* values)
{
    int notePlayed = 0;
    for (int i=0; i < BUTTON_ARRAY_SIZE; i++)
    {
        values[i] = values[i] << (BUTTON_ARRAY_SIZE - 1 - i);
        notePlayed = notePlayed | values[i];
        //printf("value%d            note%d\n", values[i], notePlayed);
    }
    return notePlayed;
}
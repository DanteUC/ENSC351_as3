#include "buttons.h"
#include "utils.h"
#include "audioMixer.h"
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
#include "control.h"


#define BUTTON_ARRAY_SIZE 4
#define RISING_EDGE "rising"

#define BUTTON_1 "/sys/class/gpio/gpio47"
#define BUTTON_2 "/sys/class/gpio/gpio46"
#define BUTTON_3 "/sys/class/gpio/gpio27"
#define BUTTON_4 "/sys/class/gpio/gpio65"
#define VALUE_PATH "/value"
#define DIRECTION_PATH "/direction"
#define ACTIVE_LOW_PATH "/active_low"
#define EDGE "/edge"
#define BUTTON_ARRAY_SIZE 4

const char* ButtonArray[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};


void ButtonArray_getButtonValues(int *values)
{
    for(int i = 0; i< BUTTON_ARRAY_SIZE; i++){
        values[i] = Utils_readIntFromFile(Utils_concat(ButtonArray[i], VALUE_PATH));
    }
}


// void* buttonsThread(void* arg)
// {

// }

//static _Bool stopping = false;
//static pthread_t buttonsThreadid;
//static pthread_mutex_t audioMutex = PTHREAD_MUTEX_INITIALIZER;



//Dr. Brian's code for edge trigger GPIO
//adjusted for multiple files
static int waitForGpioEdge(const char* fileArrayForGpioValue []) 
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

	int fdOne = 0;
	int fdTwo = 0;
	int fdThree = 0;
	int fdFour = 0;

	int fileDescriptors[] = {fdOne, fdTwo, fdThree, fdFour};
	struct epoll_event epollStruct;

	// open GPIO value file:
	for(int i = 0; i < BUTTON_ARRAY_SIZE;i++){
		fileDescriptors[i] = open(Utils_concat(fileArrayForGpioValue[i], VALUE_PATH), O_RDONLY | O_NONBLOCK);
		if (fileDescriptors[i] == -1) {
			fprintf(stderr, "ERROR: unable to open() GPIO value file (%d) = %s\n", fileDescriptors[i], strerror(errno));
			return -1;
		}

		// configure epoll to wait for events: read operation | edge triggered | urgent data
		epollStruct.events = EPOLLIN | EPOLLET | EPOLLPRI;
		epollStruct.data.fd = fileDescriptors[i];

		// register file descriptor on the epoll instance, see: man epoll_ctl
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fileDescriptors[i], &epollStruct) == -1) {
			fprintf(stderr, "ERROR: epoll_ctl() failed on add control interface (%d) = %s\n", fileDescriptors[i], strerror(errno));
			return -1;
		}
	}
	

	// ignore first trigger
	for (int i = 0; i < 2; i++) {
		int waitRet = epoll_wait(
				epollfd, 
				&epollStruct, 
				4,                // maximum # events
				-1);              // timeout in ms, -1 = wait indefinite; 0 = returne immediately

		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			for(int j = 0; i < BUTTON_ARRAY_SIZE;j++){
				close(fileDescriptors[i]);
			}
			
			close(epollfd);
			return -1;
		}
	}

	// cleanup epoll instance:
	for(int i = 0; i < BUTTON_ARRAY_SIZE;i++){
				close(fileDescriptors[i]);
			}
	close(epollfd);
	return 0;
}


void buttons_initializeButtons()
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

static int readLineFromFile(char* fileName, char* buff, unsigned int maxLength)
{
	FILE *file = fopen(fileName, "r");
	int bytes_read = getline(&buff, &maxLength, file);
	fclose(file);
	return bytes_read;
}

void buttons_pollButtons(){
	// Wait for an edge trigger:
	int ret = waitForGpioEdge(ButtonArray);
	if (ret == -1) {
		exit(EXIT_FAILURE);
	}
	
	// Current state:
	for(int i = 0; i < BUTTON_ARRAY_SIZE;i++){
		char buff[1024];
		int bytesRead = readLineFromFile(Utils_concat(ButtonArray[i], VALUE_PATH), buff, 1024);
		if (bytesRead > 0) {
			
			if(buff[0]=='1'){
				switch (i)
				{
				case 0:
					control_updateMode();
					//change beat mode
					break;
				case 1:
					AudioMixer_playBaseDrum();
					break;
				case 2:
					AudioMixer_playSnare();
					break;
				case 3: 
					AudioMixer_playHiHat();
					break;
				default:
					printf("waddahell\n");
					break;
				}
			}
			
		} else {
			fprintf(stderr, "ERROR: Read 0 bytes from GPIO input: %s\n", strerror(errno));
		}
	}
	
	Utils_sleepForMs(100);
return;
}
#include "joystick.h"


static double getJoystickInput(char* path){
    FILE* readFile = fopen(path, "r");
    if(!readFile){
        printf("error: could not open input file\n");
        exit(1);
    }
    
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, readFile);
    double value = atof(buff);
    fclose(readFile);
    return value;

}


static double Joystick_readY(){
    
    double rawValue = getJoystickInput("/sys/bus/iio/devices/iio:device0/in_voltage2_raw");
    //scale raw to be between -1.0 and 1.0
    double adjusted = (rawValue-MIDPOINT)/MIDPOINT;
    return adjusted;
}

static double Joystick_readX(){
    
    double rawValue = getJoystickInput("/sys/bus/iio/devices/iio:device0/in_voltage3_raw");
    //scale raw to be between -1.0 and 1.0
    double adjusted = (rawValue-MIDPOINT)/MIDPOINT;
    return adjusted;
}

int joystick_getDirection(){

    double X = Joystick_readX();
    double Y = Joystick_readY();

    if(Y>0.5||Y<-0.5){
        if(Y>0){
            return 2;
        }else{
            return 1;
        };
    }else if(X>0.5 || X < -0.5){
        if(X>0){
            return 4;
        }else{
            return 3;
        }
    }else{
        return 0;
    }

}



void joystick_testJoystick(){
    while(true){
        int direction = joystick_getDirection();
        printf("direction: %i\n", direction);
    }
}
/*
int main(){
    while(true){
        int direction = getDirection();
        printf("direction: %i\n", direction);
    }
    return 0;
}
*/
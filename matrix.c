#include "matrix.h"
#define DECIMAL 0b1000
#define EMPTY 0x00
static unsigned char zero[] = {0b111, 0b101, 0b101, 0b101, 0b101, 0b101, 0b111};
static unsigned char one[] = {0b110, 0b010, 0b010, 0b010, 0b010, 0b010, 0b111};
static unsigned char two[] = {0b111, 0b001, 0b001, 0b111, 0b100, 0b100, 0b111};
static unsigned char three[] = {0b111, 0b001, 0b001, 0b111, 0b001, 0b001, 0b111};
static unsigned char four[] = {0b101, 0b101, 0b101, 0b111, 0b001, 0b001, 0b001};
static unsigned char five[] = {0b111, 0b100, 0b100, 0b111, 0b001, 0b001, 0b111};
static unsigned char six[] = {0b111, 0b100, 0b100, 0b111, 0b101, 0b101, 0b111};
static unsigned char seven[] = {0b111, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001};
static unsigned char eight[] = {0b111, 0b101, 0b101, 0b111, 0b101, 0b101, 0b111};
static unsigned char nine[] = {0b111, 0b101, 0b101, 0b111, 0b001, 0b001, 0b001};


static unsigned char letterM[] = {0b101, 0b111, 0b111, 0b101, 0b101, 0b101, 0b101};



static unsigned char *digits[] = {zero, one, two, three, four, five, six, seven ,eight, nine}; 


static void writeI2CBytes(int i2cFileDesc, unsigned char regAddr, unsigned char* data, int numBytes)
{
    unsigned char buff[numBytes+1];
    buff[0] = regAddr;

    //this format is so we ignore every odd value, as the matrix rows are 0,2,4...8
    int i = numBytes-1; 
    int j = 0;
    while(i>=0){
        if(i%2 == 0){
            buff[i+1] = data[j];
            j++;
        }
        i--;
    }
    int res = write(i2cFileDesc, buff, numBytes + 1);

    if (res != numBytes + 1) {
        perror("Unable to write i2c register");
        exit(-1);
    }
}


//Dr. Brian's run command function
static void runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
        break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}


static int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

unsigned char *getBinary(int digit)
{
    return digits[digit];
}

//mode should be between 0-4
void displayMode(int mode)
{
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, BUS_ADDR);
    unsigned char displayValue[8];

    
    unsigned char *modeBits;
    if(mode > 4){
        modeBits = getBinary(4);
    }else if(mode < 0){
        modeBits = getBinary(0);
    }else{
        modeBits = getBinary(mode);
    }
    

    for(int i = 0; i < MATRIX_LENGTH; i++){
         //concat the tens digit and shifted ones digit together 
        unsigned char shifted = letterM[i]<<4 | modeBits[i];
        displayValue[i] = shifted;
    }
    //displayValue[0] = EMPTY;
    writeI2CBytes(i2cFileDesc, REG_ADDR, displayValue, NUM_BYTES);
}



void displayInteger(int value)
{
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, BUS_ADDR);
    unsigned char displayValue[7];

    unsigned char *tensBinary;
    unsigned char *onesBinary;
    
    if(value > 99){
        tensBinary = getBinary(9);
        onesBinary = getBinary(9);
    }else if(value < 0){
        tensBinary = getBinary(0);
        onesBinary = getBinary(0);
    }else if(value > 9){ 
        int tensDigit = (value/10)%10;
        int onesDigit = value %10;
        tensBinary = getBinary(tensDigit);
        onesBinary = getBinary(onesDigit);
    }else{//case single digit
        int onesDigit = value %10;
        tensBinary = getBinary(0);
        onesBinary = getBinary(onesDigit);
    }

    for(int i = 0; i < MATRIX_LENGTH; i++){
         //concat the tens digit and shifted ones digit together 
        unsigned char shifted = (tensBinary[i]<<4) | onesBinary[i];
        displayValue[i] = shifted;
    }
    writeI2CBytes(i2cFileDesc, REG_ADDR, displayValue, NUM_BYTES);
    
}
void displayDouble(double value)
{
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, BUS_ADDR);
    unsigned char displayValue[7];
    unsigned char *onesBinary;
    unsigned char *tenthsBinary;

    //set to 9.9 if value higher than 9.9
    if(value > 9.9){
        onesBinary =getBinary(9);
        tenthsBinary = getBinary(9);
    }else if(value < 0.0){
        onesBinary = getBinary(0);
        tenthsBinary = getBinary(0);
    }else{
        //multiply by 10 to more easily extract digits
        int valueAsInt = value*10;
        int onesDigit = (valueAsInt/10)%10;
        int tenthsDigit = valueAsInt %10;
        onesBinary = getBinary(onesDigit);
        tenthsBinary = getBinary(tenthsDigit);
    }

    for(int i = 0; i < MATRIX_LENGTH; i++){
        //concat the tens digit and shifted ones digit together 
        unsigned char shifted = (onesBinary[i]<<4) | tenthsBinary[i];
        displayValue[i] = shifted;
    }
    //
    displayValue[7] = DECIMAL;
    writeI2CBytes(i2cFileDesc, REG_ADDR, displayValue, NUM_BYTES);
    
}

void initializeMatrix()
{
    runCommand("i2cset -y 1 0x70 0x21 0x00");
    runCommand("i2cset -y 1 0x70 0x81 0x00");
}

/*
static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

static void unitTestMatrix()
{

    int i = -10;
    while(i<120){
        displayInteger(i);
        sleepForMs(100);
        i++;
    }
    
    double j = -2.0;
    while (j < 15.0){
        displayDouble(j);
        sleepForMs(100);
        j = j+0.1;
    }
}

main(){
    initializeMatrix();
    unitTest();
    return 0;
    }
*/

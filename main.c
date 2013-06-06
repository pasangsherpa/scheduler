/*
 * main.c
 *
 *  Created on: May 12, 2013	
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
 
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <time.h>
#include "cpu.h"

#ifndef SYSTEMTIMER_H
#include "systemtimer.h"
#endif

#define MINIMUM_NUM_PROCESS 1	//MIN NUM OF PROCESS TO RUN FOR EACH TYPE OF THE PROCESS.
#define MAX_NUM_STEPS_TO_RUN 1234567890	// MAX NUMBER OF TOTAL STEPS THIS PROGRAM WILL RUN.
#define KEYBRD_PROCESS	1

#define IO_SERVICE_CALL	7
#define PC_READ_WRITE_CALL	5
#define TIME_QUANTA	15	

/*
This function generates a 'somewhat' random number r, r >= min & r <= max
*/
int generateRandomNumber(int min, int max){
	srand(time(NULL));
	int r = (rand() % (max+1-min))+min;
	printf("random= %d \n",r);
	return r;
}

/**
This program expects arguments in this format:
-p X -k X -io X -pc X X(number of total steps CPU will count), where X is an integer.
**/
int main(int argc, char * argv[]){
 
	int processNumber = 0;
	int keyboardProcess = 0;
	int ioProcess = 0;
	int pr_coProcess = 0;	
	int computeBound = 0;
	int stepCount = 0;
	int i;
    if(argc != 10){
		printf("Please run the program again in this format:\n ./scheduler -p X -k X -io X -pc X X\nX implies an integer.");
		return 1;
	}
		
	for(i = 1 ; i < argc-1; i+=2){
			
			if(strcasecmp("-p", argv[i]) == 0){			
				processNumber = atoi(argv[i+1]);
				//create thread
			}else if(strcasecmp("-k", argv[i]) == 0){
				keyboardProcess = atoi(argv[i+1]);
			}else if(strcasecmp("-io", argv[i]) == 0){
				ioProcess = atoi(argv[i+1]);
			}else if(strcasecmp("-pc",argv[i]) == 0){
				pr_coProcess = atoi(argv[i+1]);
			}else{
				printf("Undefined argument, please try again.");
				return 1;
			}
	}
	
	stepCount = atoi(argv[argc-1]) % MAX_NUM_STEPS_TO_RUN;	
	computeBound = processNumber - (keyboardProcess+ioProcess+pr_coProcess);
		
	if(processNumber < MINIMUM_NUM_PROCESS * 4 || keyboardProcess < MINIMUM_NUM_PROCESS || ioProcess < MINIMUM_NUM_PROCESS || 	pr_coProcess < MINIMUM_NUM_PROCESS || computeBound < MINIMUM_NUM_PROCESS ){
		printf("At least %d process needs to run for each TYPE. Try again.", MINIMUM_NUM_PROCESS);
		return 1;
	}
		
	//Inputs are correct, print the initial info text to stdout:
	printf("------------------------------------\n");
	printf("Initial View\n");
	printf("-------------------------------\n");
	printf("Total number of processes: %d \n", processNumber);
	printf("Keyboard: %d\n",keyboardProcess);
	printf("I/O: %d\n",ioProcess);
	printf("Producer-Consumer Pair: %d\n",pr_coProcess);
	printf("Compute bound: %d\n",computeBound);	
	printf("Step Count: %d\n",stepCount);
	printf("------------------------------------\n");

	CPUPtr cpu = CPUConstructor();
	initCPU(cpu, processNumber, keyboardProcess, ioProcess, pr_coProcess, computeBound, stepCount);
	//Construct the timer (starts the timer)
	SysTimerConstructor(cpu->interruptController, cpu->reset);
	runCPU(cpu);

		
	return 0;
	
}




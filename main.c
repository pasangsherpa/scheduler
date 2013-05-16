/*
 * main.c
 *
 *  Created on: May 12, 2013	
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
 
# include <stdio.h>
# include <strings.h>
# include <stdlib.h>

#define MINIMUM_NUM_PROCESS 1	//MIN NUM OF PROCESS TO RUN FOR EACH TYPE OF THE PROCESS

int main(int argc, char * argv[]){
 
	int processNumber = 0;
	int keyboardProcess = 0;
	int ioProcess = 0;
	int pr_coProcess = 0;	
	int computeBound = 0;
	
    if(argc != 9){
		printf("Please run the program again in this format:\n ./scheduler -p X -k X -io X -pc X\nX implies an integer.");
		return 1;
	}
		
	for(int i = 1 ; i < argc; i+=2){	
			
			if(strcasecmp("-p", argv[i]) == 0){			
				processNumber = atoi(argv[i+1]);				
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
			
	computeBound = processNumber - (keyboardProcess+ioProcess+pr_coProcess);
		
	if(processNumber < MINIMUM_NUM_PROCESS * 4 || keyboardProcess < MINIMUM_NUM_PROCESS || ioProcess < MINIMUM_NUM_PROCESS || 	pr_coProcess < MINIMUM_NUM_PROCESS || computeBound < MINIMUM_NUM_PROCESS ){
		printf("At least %d process needs to run for each TYPE.Try again.", MINIMUM_NUM_PROCESS);
		return 1;
	}
		
	//Inputs are correct, print the initial info text to stdout:
	printf("Initial View\n");
	printf("-------------\n");
	printf("Total number of processes: %d \n", processNumber);
	printf("Keyboard: %d\n",keyboardProcess);
	printf("I/O: %d\n",ioProcess);
	printf("Producer-Consumer Pair: %d\n",pr_coProcess);
	printf("Compute bound: %d\n",computeBound);	
	
	//start
		
	return 0;
	
}


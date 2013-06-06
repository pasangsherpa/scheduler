/*
 * process.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "global.h"
#include "process.h"

//Constructor
ProcessPtr ProcessConstructor(int pid, int proc_type, int no_steps,
		int no_requests) {
	ProcessPtr process = (ProcessPtr) malloc(sizeof(ProcessStr));
	process->proc_type = proc_type;
	process->no_steps = no_steps;
	process->no_requests = no_requests;
	process->pcb = PCBConstructor(pid);
	process->requests = (RequestPtr *) malloc(sizeof(RequestStr) * no_requests);
	process->req_index = 0;
	printf("Creating process: %d (Type: %s)\n", process->pcb->pid,
			decodeProcessType(process->proc_type));

	// Set up the request array with random times!
	int i;
	int j; // Used for checking the uniqueness of random numbers.
	int r; // Random number
	bool duplicate;
	int step_array[no_requests];
	int code_array[no_requests];
	srand(time(NULL));
	for (i = 0; i < no_requests; i++) {

		if (proc_type == COMPUTE) {
			r  = -1;
		} else {
			do { // Checks for uniqueness of random number.
				duplicate = false;
				r = rand() % no_steps; // Generates a random number in the interval no_steps.

				for (j = 0; j < i; j++) {
					if (r == step_array[j]) {
						duplicate = true;
						break;
					}
				}
			} while (duplicate == true);
		}

		step_array[i] = r;

		switch (proc_type) {

			case COMPUTE:
				code_array[i] = -1;
				break;

			case IO_AUDIO:
				code_array[i] = AUDIO_SERVICE_REQ;
				break;

			case IO_VIDEO:
				code_array[i] = VIDEO_SERVICE_REQ;
				break;

			case KEYBOARD:
				code_array[i] = KEYBOARD_SERVICE_REQ;
				break;

			case PRODUCER:
			case CONSUMER:
				if (i % 2 == 1) {
					code_array[i] = MUTEX_LOCK;
				} else {
					code_array[i] = MUTEX_UNLOCK;
				}
				break;

			default:
				printf("\nInvalid process type!\n");
				break;
			}
		}

		addToRequestArray(process->requests, step_array, code_array,
				process->no_requests);

	return process;
}

//Destructor.
int ProcessDestructor(ProcessPtr this) {
	this->pcb->destroy(this->pcb);
	free(this);
	return NO_ERROR;
}

//For debugging purpose.
char* decodeProcessType(int process_type) {
	char* process;
	switch (process_type) {
	case COMPUTE:
		process = "COMPUTE";
		break;
	case KEYBOARD:
		process = "KEYBOARD";
		break;
	case PRODUCER:
		process = "PRODUCER";
		break;
	case CONSUMER:
		process = "CONSUMER";
		break;
	case IO_AUDIO:
		process = "IO_AUDIO";
		break;
	case IO_VIDEO:
		process = "IO_VIDEO";
		break;
	default:
		process = "COMPUTE";
		break;
	}
	return process;
}

/*
 This method receives 2 int arrays. req_array is the array of step nums, second is the array of trap calls.
 This method assumes both arrays have the same length.
 Constructs the requests array given an array of step nums, and trap call codes.
 */
int addToRequestArray(RequestPtr * req_array, int * num_array, int * proc_type,
		int the_length) {
	sort(num_array, the_length);
	int j = 0;
	for (j = 0; j < the_length; j++) {
		req_array[j] = RequestConstructor(proc_type[j], num_array[j]);
	}
	return NO_ERROR;
}

int getNextTrapStep(ProcessPtr this) {
	return getRequestStep(this->requests[this->req_index]);
}

int getNextTrapCode(ProcessPtr this) {
	return getRequestType(this->requests[this->req_index]);
}

bool isProcessDone(ProcessPtr this, int PC) {
	bool result = false;

	if (this -> no_steps <= PC) {
		result = true;
	}
	return result;
}

void advanceRequest(ProcessPtr this) {
	this->req_index = (this->req_index + 1) % this->no_requests;
}

int printMessage(ProcessPtr this) {
	int type = this->proc_type;

	switch (type) {

	case COMPUTE:
		printf("\nComputing");
		break;

	case IO_AUDIO:
		printf("IO Interrupt: (Audio Service Requested)\n");
		break;

	case IO_VIDEO:
		printf("IO Interrupt: (Video Service Requested)\n");
		break;

	case KEYBOARD:
		printf("IO Interrupt: (Keyboard Service Requested)\n");
		break;

	case PRODUCER:
		printf("\nProducer Interrupt");
		break;

	case CONSUMER:
		printf("\nConsumer Interrupt");
		break;

	default:
		return ERROR;
		break;
	}
	return NO_ERROR;
}

/*
 int main(void) {
 //
 //	//test for Requests:
 //	//main creates an array of request step no's and an array of trap codes
 //
 //	int  * step_array = malloc(4 * sizeof(int));  //main generates the random nums.
 //	int  * code_array = malloc(4 * sizeof(int));
 //
 //	step_array[0] = 1;
 //	step_array[1] = 8;
 //	step_array[2] = 3;
 //	step_array[3] = 9;
 //	code_array[0] = 2;
 //	code_array[1] = 4;
 //	code_array[2] = 6;
 //	code_array[3] = 8;
 //
 //
 //	//main calls the process constructor
 //	//Process type:I/0
 //	//Num of calls = 4
 //	//Step nums: 1,8,3,9
 //	//Trap codes: 2,4,6,8
 //	//number of total steps:10
 //	ProcessPtr process =  ProcessConstructor(111, IO, 10, 4) ;
 //
 //	//set requests array
 //	addToRequestArray(process->requests, step_array, code_array, process->no_requests);
 //
 //	//get next step num
 //
 //	int i;
 //	for(i = 0; i < 10; i++){
 //
 //		printf("Next trap code: %d\n", getNextTrapCode(process));
 //		printf("Next step: %d \n", getNextTrapStep(process));
 //	}
 //
 //
 // Test random generator.

 ProcessPtr process2 = ProcessConstructor(222, IO_AUDIO, 50, 5);

 int i;
 for(i = 0; i < 100; i++){

 printf("\nStep: %d", i);
 printf("\nNext Interrupt: %d \n", getNextTrapStep(process2));

 if (i == getNextTrapStep(process2)) {
 printMessage(process2);
 advanceRequest(process2);
 }

 if (isProcessDone(process2, i) == true) {
 printf("\nProcess finished!: %d\n", i);
 break;
 }
 }
 }*/


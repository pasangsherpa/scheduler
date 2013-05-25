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
#include "process.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

//Constructor
ProcessPtr ProcessConstructor(int pid, int proc_type, int no_steps, int no_requests) {
	ProcessPtr process = (ProcessPtr)malloc(sizeof(ProcessStr));
	process->proc_type = proc_type;
	process->no_steps = no_steps;
	process->no_requests = no_requests;
	process->pcb = PCBConstructor(pid);
	process->requests = (RequestPtr *) malloc(sizeof(RequestStr) * no_requests);
	process->req_index = 0;
	
	//construct the array
	return process;
};


//Destructor.
int ProcessDestructor(ProcessPtr this) {
	this->pcb->destroy(this->pcb);
	free(this);
	return NO_ERROR;
}

/*
This method receives 2 int arrays. req_array is the array of step nums, second is the array of trap calls.
This method assumes both arrays have the same length.
Constructs the requests array given an array of step nums, and trap call codes.
*/
int addToRequestArray(RequestPtr * req_array, int * num_array, int * proc_type, int the_length){
	sort(num_array, the_length);
	int j = 0;
	for(j = 0; j < the_length ; j++){
		req_array[j] = RequestConstructor(proc_type[j],num_array[j]);		
	}
	return NO_ERROR;
}

int getNextTrapStep(ProcessPtr this){
	int result =  getRequestStep(this->requests[this->req_index]);
	this->req_index = (this->req_index +1 ) % this->no_requests;
	return result;
}

int getNextTrapCode(ProcessPtr this){
	 return getRequestType(this->requests[this->req_index]);	
}
	

//Run the process depending on its type.
int run(ProcessPtr this) {
	int type = this->proc_type;
	switch (type) {
		case COMPUTE:
			//Create a compute process.
			break;
		case IO:
			//Create a IO process.
			break;
		case KEYBOARD:
			//Create a Keyboard process.
			break;
		case PRODUCER:
			//Create a Producer process.
			break;
		case CONSUMER:
			//Create a Consumer process.
			break;
		default:
			break;
	}
	return NO_ERROR;
}

/*int main(void){

	//test for Requests:
	//main creates an array of request step no's and an array of trap codes
	
	int  * step_array = malloc(4 * sizeof(int));  //main generates the random nums.
	int  * code_array = malloc(4 * sizeof(int));
	
	step_array[0] = 1;
	step_array[1] = 8;
	step_array[2] = 3;
	step_array[3] = 9;
	code_array[0] = 2;
	code_array[1] = 4;
	code_array[2] = 6;
	code_array[3] = 8;
	
	
	//main calls the process constructor
	//Process type:I/0
	//Num of calls = 4
	//Step nums: 1,8,3,9
	//Trap codes: 2,4,6,8
	//number of total steps:10
	ProcessPtr process =  ProcessConstructor(111, IO, 10, 4) ;
	
	//set requests array
	addToRequestArray(process->requests, step_array, code_array, process->no_requests);
	
	//get next step num
	int i;
	for(i = 0; i < 10; i++){
	
	printf("Next trap code: %d\n", getNextTrapCode(process));
	printf("Next step: %d \n", getNextTrapStep(process));
	}
	
	return 0;
}
*/	





#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "global.h"
#include "process.h"
#include "pcb.h"
#include "scheduler.h"
#include "TestCpu.h"

CPUPtr CPUConstructor(int the_max_step_count, pthread_cond_t condition) {
		CPUPtr result = (CPUPtr) malloc(sizeof(CPUStr));
		result->PC = 0;										//current process' PC
		result->max_step_count = the_max_step_count;		//max steps this CPU will run
		result->next_process = 0;							//next service call address in the trap vector
		result->next_step=0;								//next PC when currently running process will be preemted.
		result->INT =0;										//whether this cpu is interrupted.
		result->IRQ = 0;									//IRQ code of the interrupting device
		result->resume = 0;									//may not be necessary
		result->buffer_data = '0';							//buffer
		result->process_pid = 0;							//current process pid
		result -> reset = condition;

		return result;
}

void interruptCPU(CPUPtr this, int the_IRQ, char the_data) {
	this->INT = 1;
	this->IRQ = the_IRQ;
	this->buffer_data = the_data;
	printf("\nSignaling timer");
	pthread_cond_signal(&this -> reset);
}
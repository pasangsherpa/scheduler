/*
 * cpu.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include "cpu.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

CPUPtr CPUConstructor(int no_processes, int prod_cons_pair, int cycle) {
        CPUPtr cpu = (CPUPtr) malloc(sizeof(CPUStr));
        cpu->PC = 0;
        cpu->memory = MemoryConstructor();
        cpu->scheduler = SchedulerConstructor(8);
        cpu->no_processes = no_processes;
        cpu->process_list = (ProcessPtr*)(malloc(MAX_PROCESS *(sizeof(ProcessStr))));
		cpu->keyboard_queue = (Queue) CreateQueue(QUEUE_SIZE);
        //Pointers to functions.

        return cpu;
}

int keyboardServiceRequest(CPUPtr this, SchedulerPtr the_scheduler){
		printf("Keyboard thread\n");
		ProcessPtr pcb = getCurrentProcess(the_scheduler); //PCBPtr//	
			
		Enqueue(pcb, this-> keyboard_queue); //Enqueue the PCB to the I/O queue.
		//Process state is set to BLOCKED.
		//Process state is saved.
		//PCB is removed from the READY queue.		
		//upcall the scheduler
		setCurrentProcess(the_scheduler);
		printf("cpu disabled int,saved the state,enabled int, and moved the pcb to the Keyboard's blocked queue.\n");		
		printf("CPU upcalled the Scheduler\n");
		poll(this);	
		//cpu resume
		
		return 0;
}

int poll(CPUPtr this){ //thread
	char c = 'a';
	keyboardServiceCompleted(this, c);
	return NO_ERROR;
}
																
int keyboardServiceCompleted(CPUPtr this, char c){
	printf("CPU has been interrupted by the Keyboard Device.\n");
	ProcessPtr pcb = FrontAndDequeue(this->keyboard_queue);
	int result = addToReadyQueue(this->scheduler, pcb);
	//if OK
	printf("CPU disabled INT,saved the state of the current process to its PCB, enabled int, and moved the completed process to the ready queue.\n");
	printf("Character from the buffer is %c\n",c);	
	//cpu resume
	return NO_ERROR;
	}

//Destructor.
int CPUDestructor(CPUPtr this) {
        free(this);
        return NO_ERROR;
}

int main (void){
	CPUPtr cpu = CPUConstructor(6,6,6);	
	keyboardServiceRequest(cpu, cpu->scheduler);
	return 0;
}
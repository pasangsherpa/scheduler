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
        cpu->scheduler = SchedulerConstructor(MAX_PROCESS);
        cpu->no_processes = no_processes;
        cpu->process_list = (ProcessPtr*)(malloc(MAX_PROCESS *(sizeof(ProcessStr))));
		cpu->keyboard_queue = (Queue) CreateQueue(QUEUE_SIZE);
        //Pointers to functions.

        return cpu;
}

int keyboardServiceRequest(CPUPtr this, SchedulerPtr the_scheduler){ //a key press should trigger this method.
		
		ProcessPtr curr_process = getCurrentProcess(the_scheduler); 		
		curr_process->pcb->state = BLOCKED;		//process is blocked
		curr_process->no_steps = this->PC;		//context saved			
		Enqueue(curr_process, this-> keyboard_queue); //Enqueue the PCB to the I/O queue.
		printf("------------------I/O REQUEST---------------------\n");
		printf("Process PID = %d requested a keyboard service.\n",curr_process->pcb->pid);
		printf("Process PID = %d's PC = %d has been saved to its PCB\n",curr_process->pcb->pid,curr_process->no_steps);		
		setCurrentProcess(the_scheduler);
		printf("Process PID = %d - running\n", getCurrentProcess(the_scheduler)->pcb->pid);	
		printf("------------------I/O REQUEST---------------------\n");
		printf("Enter char: \n");
		poll(this);			
		return 0;
}

void poll(CPUPtr this){ 
		char stdin_char = getchar();
		getchar();
		keyboardServiceCompleted(this, stdin_char);		
}
																
int keyboardServiceCompleted(CPUPtr this, char c){
	printf("------------------I/O INT---------------------\n");
	printf("I/O Completion Interrupt[Keyboard Device with IRQ = %d]\n", IRQ_KEYBOARD);
	ProcessPtr completed_process = FrontAndDequeue(this->keyboard_queue); //process removed from keyboard queue
	printf("P%d selected from Keyboard I/O Queue\n",completed_process->pcb->pid);
	printf("P%d returned to Ready Queue\n",completed_process->pcb->pid);
	printf("Key pressed =  %c\n",c);
	printf("P%d- running\n",getCurrentProcess(this->scheduler)->pcb->pid);
	printf("------------------I/O INT---------------------\n");
	int result = addToReadyQueue(this->scheduler, completed_process);
	return result;	
}

/*
Prints out the state of the simulation: Running process, queues etc.
*/
void printState(CPUPtr this){
	ProcessPtr curr_process = getCurrentProcess(this->scheduler);
	printf("------------------State---------------------\n");
	printf("P%d - running\n", curr_process->pcb->pid);
	printf("Keyboard Device I/O Queue -");
	printQueue(this->keyboard_queue);
	printf("\n");
	printf("Scheduler Ready Queue -");
	printQueue(this->scheduler->ready_queue);
	printf("\n");
	printf("Audio Device I/O Queue -");
	//printQueue(this->audio_queue);
	printf("waiting\n");
	printf("Video Device I/O Queue -");
	//printQueue(this->video_queue);
	printf("\n");
	printf("M%d - P%d owns, ", 1,1);
	//printQueue(mutex1_queue);
	printf("waiting\n");
	printf("MV%d - P%d owns, ",1,1);
	//printQueue(cond1_queue);
	printf("waiting\n");
	printf("----------------------------------------------------\n");
}
	

//Destructor.
int CPUDestructor(CPUPtr this) {
        free(this);
        return NO_ERROR;
}

/*
Prints out the PID of the processes in the given queue
*/
void printQueue(Queue the_queue){
	
	if(IsEmpty(the_queue) == 0){
	
		ElementType * temp = getQueue(the_queue);
		int list_size = Size(the_queue);			
			ProcessPtr p ;
			int index = 0;
			for(index = 0; index < list_size; index++){
				p = temp[index];				
				printf("PID = %i  ",p->pcb->pid);
			}		
	}
}

int main (void){

	//Test Case: A Process calls a keyboard service.
	//State of the program 
	//Pressed key is printed out.
	//Context switch
	
	//Initial prep
	CPUPtr cpu = CPUConstructor(6,6,6);		
	ProcessPtr process1 = ProcessConstructor(111,IO,1,3);
	ProcessPtr process2 = ProcessConstructor(222,IO,1,3);	
	addToReadyQueue(cpu->scheduler, process1); 
	addToReadyQueue(cpu->scheduler, process2);
	setCurrentProcess(cpu->scheduler);
	//printf("Process is set %d\n",setCurrentProcess(cpu->scheduler)); //Initialization.
	
	//state
	printState(cpu);
	
	//calls the service
	keyboardServiceRequest(cpu,cpu->scheduler);
	
	//state
	printState(cpu);	
	
	return 0;
}
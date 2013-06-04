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


CPUPtr CPUConstructor() {
	CPUPtr result = (CPUPtr) malloc(sizeof(CPUStr));
	result->PC = 0; //current process' PC
	result->next_process = 0; //next service call address in the trap vector
	result->next_step = 0; //next PC when currently running process will be preemted.
	result->current_process = NULL; //currently running process
	result->INT = 0; //whether this cpu is interrupted.
	result->IRQ = 0; //IRQ code of the interrupting device
	result->buffer_data = '0'; //buffer
	result->process_pid = 0; //current process pid
	return result;
}

//Initialize CPU.
void initCPU(CPUPtr this, int totalProcess, int totalKBProcess,
		int totalIOProcess, int totalPrCoProcess, int totalComputeProcess,
		int the_max_step_count) {
	this->reset = PTHREAD_COND_INITIALIZER;
	this->scheduler = SchedulerConstructor(totalProcess);//process scheduler
	this->max_step_count = the_max_step_count; //max steps this CPU will run

	//Setup all processes
	int i;
	int pid = 0;

		for (i = 0; i < totalIOProcess; i++) {
			int processtype = IO_AUDIO;
			if (i % 2 == 1)
				processtype = IO_VIDEO; // Create IO_VIDEO every other number.
			ProcessPtr iop = ProcessConstructor(pid, processtype, 300, 100);
			this->scheduler->addToQueue(this->scheduler, iop,
					this->scheduler->ready_queue);
			pid++;
		}

	for (i = 0; i < totalComputeProcess; i++) {
		ProcessPtr cm = ProcessConstructor(pid, COMPUTE, 300, 100);
		this->scheduler->addToQueue(this->scheduler, cm,
				this->scheduler->ready_queue);
		pid++;
	}

	for (i = 0; i < totalKBProcess; i++) {
			ProcessPtr kbp = ProcessConstructor(pid, KEYBOARD, 300, 100); // 300 & 100 dummy values
			this->scheduler->addToQueue(this->scheduler, kbp,
					this->scheduler->ready_queue);
			pid++;
		}

		for (i = 0; i < totalPrCoProcess; i++) {
			int processtype = PRODUCER;
			if (i % 2 == 1)
				processtype = CONSUMER;
			ProcessPtr prcp = ProcessConstructor(pid, processtype, 300, 100);
			this->scheduler->addToQueue(this->scheduler, prcp,
					this->scheduler->ready_queue);
			pid++;
	}

	printf("------------------------------------\n");
	//Load first process in the scheduler.
	this->scheduler->setCurrentProcess(this->scheduler);
	//set the currect process in cpu.
	this->current_process
			= this->scheduler->getCurrrentProcess(this->scheduler);

	ProcessPtr p = this->scheduler->getCurrrentProcess(this->scheduler);
	printf("Current: %d\n", p->pcb->pid);

	//Construct the timer (starts the timer)
	this->timer = SysTimerConstructor((CPU) this, this->reset);
}

/*
 Context switch: CPU's context is switched to the next process selected by the process scheduler.
 */
void setNextProcess(CPUPtr this) {
	this->current_process = getCurrentProcess(this->scheduler);
	this->PC = this->current_process->no_steps;
	this->next_step = getNextTrapStep(this->current_process);
	this->next_process = getNextTrapCode(this->current_process);
	this->process_pid = this->current_process->pcb->pid;
}

/*
 Devices call this method when they send INT signal to the CPU.
 */
void interruptCPU(CPUPtr this, int the_IRQ, char the_data) {
	this->INT = 1;
	this->IRQ = the_IRQ;
	this->buffer_data = the_data;
}

/*
 CPU thread runs as long as there are more steps to run.
 */
void runCPU(CPUPtr this) { //main thread.//assumes that the fields are set
	while (this->max_step_count > 0) {
		this->max_step_count--;//decrement the max step
		this->PC = this->current_process->pcb->next_step;
		printf("Current count = %d\n", this->max_step_count);

		if (this->INT == 1) {
			printf("\nINT: %d\n", this -> IRQ);

			switch (this->IRQ) {
			case TIMER_INT:
				printf("Timer interrupt/n");
				//saveState(this);
				//switchProcess(this->scheduler, this->PC, 1);
				pthread_cond_signal(&this->reset);

				break;
			case AUDIO_SERVICE_COMPLETED: //no context switch
				//saveState(this);
				//switchProcess(this->scheduler, this->PC, 7);//QUEUE IS OK

				break;
			case VIDEO_SERVICE_COMPLETED://no context switch
				//saveState(this);
				//switchProcess(this->scheduler, this->PC, 3);

				break;
			case KEYBOARD_COMPLETED://no context switch
				saveState(this);
				keyboardServiceCompleted(this);
				break;
			default:
				printf("INT not recognized\n");
			}

			this -> INT = 0;
		}

		if (this->PC == getNextTrapStep(this -> current_process)) { //time to make a service call
			printf("Service Call made...\n");
			printMessage(this ->current_process);
			advanceRequest(this -> current_process);
			interruptCPU(this, getNextTrapCode(this ->current_process), '0');

			/*
			 this->resume = 0;
			 switch(this->next_process){
			 case VIDEO_SERVICE_REQ: //context switch
			 //saveState(this); //create video service thread
			 //switchProcess(this->scheduler, this->PC, 2);
			 break;
			 case KEYBOARD_SERVICE_REQ://special case //context switch
			 saveState(this);
			 printf("Saved state \n");
			 keyboardServiceRequest(this);
			 break;
			 case AUDIO_SERVICE_REQ: //context switch //create audio service thread
			 //saveState(this);
			 //switchProcess(this->scheduler, this->PC, 6);
			 break;
			 case MUTEX_LOCK:
			 //saveState(this);
			 //switchProcess(this->scheduler, this->PC, 8);
			 break;
			 case MUTEX_UNLOCK:
			 //saveState(this);
			 //switchProcess(this->scheduler, this->PC, 9);
			 break;
			 case COND_WAIT: //context switch
			 //saveState(this);
			 //switchProcess(this->scheduler, this->PC, 10);
			 break;
			 case COND_SIGNAL:
			 //saveState(this);
			 //switchProcess(this->scheduler, this->PC, 11);
			 break;
			 default:
			 printf("TRAP not recognized");
			 }*/

		}

		if (isProcessDone(this ->current_process, this -> PC)) {
			printf("Process Completed..\n");
			this -> PC = 0;
		}
	}
	printf("done done done");

	//kill cpu
}

void saveState(CPUPtr this) {
	this->current_process->no_steps = this->PC;
}

void keyboardServiceRequest(CPUPtr this) { //a key press should trigger this method.
	printf("------------------I/O REQUEST---------------------\n");
	printf("Process PID = %d requested a keyboard service.\n",
			this->process_pid);
	printf("Process PID = %d's PC = %d has been saved to its PCB\n",
			this->process_pid, this->PC);
	switchProcess(this->scheduler, &this->PC, 4, NULL);
	setNextProcess(this);
	printf("Process PID = %d - running\n", this->process_pid);
	printf("------------------I/O REQUEST---------------------\n");
	printf("Enter char: \n");
	getKey(this);

}

void keyboardServiceCompleted(CPUPtr this) {
	printf("------------------I/O INT---------------------\n");
	printf("I/O Completion Interrupt[Keyboard Device with IRQ = %d]\n",
			IRQ_KEYBOARD);
	//get the pid of the process(I/O queue) from the scheduler.
	switchProcess(this->scheduler, &this->PC, 5, NULL);
	printf("Key pressed =  %c\n", this->buffer_data);
	printf("P%d- running\n", this->process_pid);
	printf("------------------I/O INT---------------------\n");
}

/*
 The Keyboard Device.
 */
void getKey(CPUPtr this) {
	char stdin_char = getchar();
	getchar();
	interruptCPU(this, 1, stdin_char);
}

/*
 Prints out the state of the simulation: Running process, queues etc.
 */
void printState(CPUPtr this) {
	ProcessPtr curr_process = getCurrentProcess(this->scheduler);
	printf("------------------State---------------------\n");
	printf("P%d - running\n", curr_process->pcb->pid);
	printf("Keyboard Device I/O Queue -");
	//printQueue(this->keyboard_queue);    //Get queue from the scheduler
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
	printf("M%d - P%d owns, ", 1, 1);
	//printQueue(mutex1_queue);
	printf("waiting\n");
	printf("MV%d - P%d owns, ", 1, 1);
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
void printQueue(Queue the_queue) {

	if (IsEmpty(the_queue) == 0) {

		ElementType * temp = getQueue(the_queue);
		int list_size = Size(the_queue);
		ProcessPtr p;
		int index = 0;
		for (index = 0; index < list_size; index++) {
			p = temp[index];
			printf("PID = %i  ", p->pcb->pid);
		}
	}
}

//int main (void){
//
//	//Test Case: A Process calls a keyboard service.
//	//State of the program
//	//Pressed key is printed out.
//	//Context switch
//
//	//Initial prep
//	//CPU
//	CPUPtr cpu = CPUConstructor(30);
//
//	//Construct 2 Processes
//
//		//main creates an array of request step no's and an array of trap codes
//
//		int  * step_array = malloc(4 * sizeof(int));  //main generates the random nums.
//		int  * code_array = malloc(4 * sizeof(int));
//
//			step_array[0] = 1;
//			step_array[1] = 8;
//			step_array[2] = 3;
//			step_array[3] = 9;
//			code_array[0] = 4;
//			code_array[1] = 6;
//			code_array[2] = 8;
//			code_array[3] = 4;
//
//
//		//main calls the process constructor
//			//Process type:I/0
//			//Num of calls = 4
//			//Step nums: 1,8,3,9
//			//Trap codes: 2,4,6,8
//			//number of total steps:10
//		ProcessPtr process1 =  ProcessConstructor(111, IO, 10, 4) ;
//
//		//set requests array
//		addToRequestArray(process1->requests, step_array, code_array, process1->no_requests);
//
//
//			//Process type:I/0
//			//Num of calls = 4
//			//Step nums: 3,5,1,9
//			//Trap codes: 8,9,10,11
//			//number of total steps:10
//			step_array[0] = 3;
//			step_array[1] = 5;
//			step_array[2] = 1;
//			step_array[3] = 9;
//			code_array[0] = 8;
//			code_array[1] = 9;
//			code_array[2] = 10;
//			code_array[3] = 11;
//
//			ProcessPtr process2 = ProcessConstructor(222,IO,10,4);
//			addToRequestArray(process2->requests, step_array, code_array, process2->no_requests);
//
//
//
//	 //main adds the processes to the scheduler's queue.
//		addToQueue(cpu->scheduler, process1, cpu->scheduler->ready_queue);
//		addToQueue(cpu->scheduler, process2, cpu->scheduler->ready_queue);
//
//	//get CPU's initial process to run.
//		setCurrentProcess(cpu->scheduler);
//		setNextProcess(cpu); //load selected process state to the cpu.
//
//	//CPU RUN
//	runCPU(cpu);
//
//
//	return 0;
//}
/*
int main() {
	CPUPtr cpu = CPUConstructor(30);

}*/

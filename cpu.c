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

#include <pthread.h>
#include "cpu.h"


CPUPtr CPUConstructor() {
	CPUPtr result = (CPUPtr) malloc(sizeof(CPUStr));
	result->PC = -1; // Initial PC (will be incremented to 0).
	result->current_process = NULL; //currently running process
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
	this->interruptController = ICConstructor();
	this->max_step_count = the_max_step_count; //max steps this CPU will run

	//Setup all processes
	int i;
	int pid = 0;

	for (i = 0; i < totalIOProcess; i++) {
		int processtype = IO_AUDIO;
		if (i % 2 == 1)
			processtype = IO_VIDEO; // Create IO_VIDEO every other number.
		ProcessPtr iop = ProcessConstructor(pid, processtype, 6, 2);
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

	printf("Total process created = %d\n", totalProcess);

	printf("------------------------------------\n");
	//Load first process in the scheduler.
	this->scheduler->setCurrentProcess(this->scheduler);
	//set the currect process in cpu.
	this->current_process
			= this->scheduler->getCurrrentProcess(this->scheduler);

	// Start timer.
	this -> timer = (SysTimerPtr) SysTimerConstructor(this -> interruptController, this -> reset);
}

/*
 Context switch: CPU's context is switched to the next process selected by the process scheduler.
 */
void setNextProcess(CPUPtr this) {
	this->current_process = getCurrentProcess(this->scheduler);
	this->process_pid = this->current_process->pcb->pid;
}


/*
 CPU thread runs as long as there are more steps to run.
 */
void runCPU(CPUPtr this) { //main thread.//assumes that the fields are set
	while (this->max_step_count > 0) {
		this->PC++; // Increment the PC.
		printf("Current count = %d\n", this->max_step_count);
		printf("PC Value: %d\n", this->PC);
		printf("Process %d (Type: %s) is running... \n", this->current_process->pcb->pid,
				decodeProcessType(this->current_process->proc_type));
		/*
		 * Checks to see whether an interrupt has occured.
		 * If so, it figures out what generated it and takes appropriate action.
		 */
		if (isInterruptWaiting(this->interruptController)) {

			// Retrieve the interrupt.
			interruptPtr interrupt = retrieveInterrupt(this->interruptController);

			switch (interrupt->the_irq) { // Figure out which interrupt has occured.

			case TIMER_INT:
				printf("Timer interrupt/n");
				switchProcess(this->scheduler, &this->PC, TIMER_INT, NULL);

				//  Signal to restart the timer.
				pthread_cond_signal(&this->reset);
				break;

			case VIDEO_SERVICE_REQ:
				switchProcess(this->scheduler, &this->PC, VIDEO_SERVICE_REQ,
						NULL);
				IODeviceConstructor(this->interruptController, IO_VIDEO);
				break;

			case VIDEO_SERVICE_COMPLETED: //no context switch
				printf("IO: (Video Service Completed)\n");
				switchProcess(this->scheduler, &this->PC,
						VIDEO_SERVICE_COMPLETED, NULL);
				break;

			case KEYBOARD_SERVICE_REQ:
				switchProcess(this->scheduler, &this->PC, KEYBOARD_SERVICE_REQ,
						NULL);
				break;

			case KEYBOARD_SERVICE_COMPLETED:
				printf("IO: (Keyboard Service Completed)\n");
				switchProcess(this->scheduler, &this->PC,
						KEYBOARD_SERVICE_COMPLETED, NULL);
				break;

			case AUDIO_SERVICE_REQ:
				printf("In Audio Service Request\n");
				switchProcess(this->scheduler, &this->PC, AUDIO_SERVICE_REQ,
						NULL);
				IODeviceConstructor(this->interruptController, IO_AUDIO);
				break;

			case AUDIO_SERVICE_COMPLETED: //no context switch
				printf("IO: (Audio Service Completed)\n");
				switchProcess(this->scheduler, &this->PC,
						AUDIO_SERVICE_COMPLETED, NULL);
				break;

			case MUTEX_LOCK:
				printf("Mutex Lock Requested\n");
				// ADD additional
				break;

			case MUTEX_UNLOCK:
				printf("Mutex Unlock Requested\n");
				// ADD additional
				break;

			case COND_WAIT:
				printf("Waiting on Condition Variable\n");
				// ADD additional
				break;

			case COND_SIGNAL:
				printf("Condition Variable Signaled\n");
				// ADD additional
				break;

			default:
				printf("INT not recognized\n");
				break;
			}

			setNextProcess(this);
			printf("pid after context switch: %d\n", this -> process_pid);

			// Destroy the interrupt.
			interruptDestructor(interrupt);
		}

		/*
		 * Checks to see if the current process is going to make a service call.
		 * If so, it prints its message, advances its request queue, and
		 * interrupts the CPU.
		 */
		else if (this->PC == getNextTrapStep(this -> current_process)) {
			printf("\nAbout to make a service call...\n");
			printMessage(this ->current_process);
			advanceRequest(this -> current_process);
			interruptCPU(this->interruptController,
										getNextTrapCode(this ->current_process), '0');
		}

		/*
		 * Checks to see whether a process has completed its total number
		 * of steps. If so, it prints that its completed and resets the PC
		 * to the beginning of the program
		 */
		if (isProcessDone(this ->current_process, this -> PC)) {
			printf("Process %d Completed..\n", this->current_process->pcb->pid);
			this -> PC = -1; // Will be incremented to 0 at top of loop.
		}

		this->max_step_count--; //decrement the max step.
	}

	printf("\nThere are no more tasks in the ready queue.");

	//kill cpu
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
	interruptCPU(this->interruptController, 1, stdin_char);
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
	//	printQueue(this->scheduler->ready_queue);
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

/**
 * Print Queue
 */
void printQueueElement(CPUPtr this, int q_type) {
	char* name;
	Queue Q;
	int i;
	switch (q_type) {
	case (READY_QUEUE):
		Q = this->scheduler->ready_queue;
		name = "Ready Queue";
		break;
	case (IO_QUEUE):
		Q = this->scheduler->io_queue;
		name = "IO Queue";
		break;
	case (MUTEX_QUEUE):
		//		Q = this->mutex->mutex_queue;
		name = "Mutex Queue";
		break;
	default:
		Q = this->scheduler->ready_queue;
		name = "Ready Queue";
		break;
	}
	printf("size rq: %d\n\n", Size(this->scheduler->ready_queue));

	ProcessPtr* p = (ProcessPtr*) getQueue(Q);

	printf("%s = ", name);
	for (i = 0; i < Size(Q); i++) {
//		printf("%d:", p[i]->pcb->pid);
	}

	printf("\n");
}



//Destructor.
int CPUDestructor(CPUPtr this) {
	free(this);
	return NO_ERROR;
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

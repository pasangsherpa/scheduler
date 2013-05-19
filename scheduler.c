/*
 * scheduler.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include <stdio.h>
#include "scheduler.h"

#ifndef  GLOBAL_H
#include "global.h"
#endif

//Constructor
SchedulerPtr SchedulerConstructor(int max_processes) {
	SchedulerPtr scheduler = (SchedulerPtr) malloc(sizeof(SchedulerStr));
	scheduler->ready_queue = CreateQueue(max_processes);

	//Pointers to functions.
	scheduler->destroy = SchedulerDestructor;
	scheduler->setCurrentProcess = setCurrentProcess;
	scheduler->getCurrrentProcess = getCurrentProcess;
	scheduler->addToReadyQueue = addToReadyQueue;
	scheduler->switchProcess = switchProcess;

	return scheduler;
}

//Destructor
int SchedulerDestructor(SchedulerPtr this) {
	DisposeQueue(this->ready_queue);
	free(this);
	return NO_ERROR;
}

/**
 * Set current running process.
 */
int setCurrentProcess(SchedulerPtr this) {
	if (IsEmpty(this->ready_queue)) {
		return ERROR;
	}
	this->current_process = FrontAndDequeue(this->ready_queue);
	return NO_ERROR;
}

/**
 * Switch the current running process with the next process in readyqueue and
 * return the new current running process.
 */
ProcessPtr switchProcess(SchedulerPtr this, int PC) {
	ProcessPtr process = this->current_process;
	switch (process->pcb->state) {
		case RUNNING:
			process->pcb->state = READY;
			break;
		default:
			break;
	}
	addToReadyQueue(this, process);
	this->setCurrentProcess(this);
	if (this->setCurrentProcess != NULL) {
		this->current_process->pcb->next_step = PC;
		this->current_process->pcb->state = RUNNING;
		return this->current_process;
	}
	return NULL;
}

/**
 * Add a process to the Ready Queue.
 */
int addToReadyQueue(SchedulerPtr this, ProcessPtr process) {
	if (IsFull(this->ready_queue)) {
		return ERROR;
	}
	Enqueue(process, this->ready_queue);
	return NO_ERROR;
}

/**
 * Get the current running process.
 */
ProcessPtr getCurrentProcess(SchedulerPtr this) {
	return this->current_process;
}

/**
 * Test Scheduler
 */
void testScheduler(SchedulerPtr this) {

	printf("Testing\n");
	ProcessPtr p1 = ProcessConstructor(001, COMPUTE, 50, 10);
	p1->pcb->state = RUNNING;
	ProcessPtr p2 = ProcessConstructor(002, IO, 50, 10);
	p2->pcb->state = INTERRUPTED;
	ProcessPtr p3 = ProcessConstructor(003, KEYBOARD, 50, 10);
	p3->pcb->state = BLOCKED;
	p3->pcb->waiting_on = 1; //P3 waiting on mutex 1;
	ProcessPtr p4 = ProcessConstructor(004, PRODUCER, 50, 10); //Default state is READY
	p4->pcb->owns = 1; // P4 owns mutex 1;
	ProcessPtr p5 = ProcessConstructor(005, CONSUMER, 50, 10); //Default state is READY

	this->addToReadyQueue(this, p4);
	this->addToReadyQueue(this, p5);
	printf("Ready Queue size before setting current process: %d\n", Size(this->ready_queue));

	this->setCurrentProcess(this); //Initialization.
	printf("Ready Queue size after setting current process: %d\n", Size(this->ready_queue));
	printf("Current Process: %d\n", this->current_process->pcb->pid);
	ProcessPtr p = this->current_process;
	printf("Process %d's status before interrupt is %d \n", p->pcb->pid, p->pcb->state);
	int PC = 999;
	printf("Timer Interrupt......\n");
	this->current_process->pcb->state = INTERRUPTED;
	this->switchProcess(this, PC);
	printf("Ready Queue size after switching process: %d\n", Size(this->ready_queue));
	printf("Current Process: %d\n", this->current_process->pcb->pid);
	printf("Process %d's status before interrupt is %d \n", p->pcb->pid, p->pcb->state);

}

//int main(void) {
//	SchedulerPtr slr = SchedulerConstructor(10);
//	testScheduler(slr);
//
//	return 0;
//}

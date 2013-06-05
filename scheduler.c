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
	scheduler->io_queue = CreateQueue(max_processes);
	//scheduler->mutex = MutexConstructor();

	//Pointers to functions.
	scheduler->destroy = SchedulerDestructor;
	scheduler->setCurrentProcess = setCurrentProcess;
	scheduler->getCurrrentProcess = getCurrentProcess;
	scheduler->addToQueue = addToQueue;
	scheduler->switchProcess = switchProcess;

	return scheduler;
}

//Destructor
int SchedulerDestructor(SchedulerPtr this) {
	DisposeQueue(this->ready_queue);
	DisposeQueue(this->io_queue);
	free(this);
	return NO_ERROR;
}

void printQueueElement(Queue queue) {
		int i;
		ProcessPtr* q = (ProcessPtr*) getQueue(queue);
		i = 0;
		while (i < Size(queue)) {
			printf("%d\n", q[i]->pcb->pid);
			i++;
	}

 }
/**
 * Set current running process.
 */
int setCurrentProcess(SchedulerPtr this) {
	if (IsEmpty(this->ready_queue)) {
		return ERROR;
	}
	this->current_process = FrontAndDequeue(this->ready_queue);
	this->current_process->pcb->state = RUNNING;
	return NO_ERROR;
}

/**
 * Switch the current running process with the next process in readyqueue and
 * return the new current running process.
 * Assumes all saving context is taken care of before making call to this function.
 */
ProcessPtr switchProcess(SchedulerPtr this, int *PC, int interrupt, ProcessPtr prcss) {
	ProcessPtr process = NULL;

	switch (interrupt) {
	case TIMER_INT:
		this->addToQueue(this, this->current_process, this->ready_queue);
		this->current_process->pcb->state = READY;
		this->current_process->no_steps = *PC; //Store the current PC
		this->setCurrentProcess(this);
		*PC = this->current_process->no_steps; //Start where it was left at
		break;
	case VIDEO_SERVICE_REQ:
	case AUDIO_SERVICE_REQ:
	case KEYBOARD_SERVICE_REQ:
		this->addToQueue(this, this->current_process, this->io_queue);
		this->current_process->pcb->state = BLOCKED; //waiting on io
		this->current_process->no_steps = *PC - 1; //Store the current PC
		this->setCurrentProcess(this);
		*PC = this->current_process->no_steps; //Start where it was left at
		break;
	case VIDEO_SERVICE_COMPLETED:
	case AUDIO_SERVICE_COMPLETED:
	case KEYBOARD_SERVICE_COMPLETED:
		if (!IsEmpty(this->io_queue)) {
			process = FrontAndDequeue(this->io_queue);
			process->pcb->state = READY;
			this->addToQueue(this, process, this->ready_queue);
		}
		break;
	case MUTEX_LOCK:  //if a process upcalls the scheduler for mutex lock, we know that it is blocked.

			this->current_process->pcb->state = BLOCKED;
			FrontAndDequeue(this->ready_queue);//the process needs to be removed from the ready queue.
			this->setCurrentProcess(this);

		break;
	case MUTEX_UNLOCK:
			prcss->pcb->state = READY;
			this->addToQueue(this,prcss,this->ready_queue);
		break;

	case COND_WAIT:
		break;

	case COND_SIGNAL:
		break;
	default:
		break;
	}

	return this->current_process;
}

/**
 * Add a process to the Queue.
 */
int addToQueue(SchedulerPtr this, ProcessPtr process, Queue Q) {
	if (IsFull(Q)) {
		return ERROR;
	}
	Enqueue(process, Q);
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
	ProcessPtr p2 = ProcessConstructor(002, IO_AUDIO, 50, 10);
	p2->pcb->state = INTERRUPTED;
	ProcessPtr p3 = ProcessConstructor(003, KEYBOARD, 50, 10);
	p3->pcb->state = BLOCKED;
	p3->pcb->waiting_on = 1; //P3 waiting on mutex 1;
	ProcessPtr p4 = ProcessConstructor(004, PRODUCER, 50, 10); //Default state is READY
	p4->pcb->owns = 1; // P4 owns mutex 1;
	ProcessPtr p5 = ProcessConstructor(005, CONSUMER, 50, 10); //Default state is READY

	this->addToQueue(this, p4, this->ready_queue);
	this->addToQueue(this, p5, this->ready_queue);
	ProcessPtr* q = (ProcessPtr*) getQueue(this->ready_queue);

	int i = 0;
	while (i < Size(this->ready_queue)) {
		printf("%d\n", q[i]->pcb->pid);
		i++;
	}

	printf("Ready Queue size before setting current process: %d\n",
			Size(this->ready_queue));

	this->setCurrentProcess(this); //Initialization.
	printf("Ready Queue size after setting current process: %d\n",
			Size(this->ready_queue));
	printf("Current Process: %d\n", this->current_process->pcb->pid);
	ProcessPtr p = this->current_process;
	int PC = 999;
	printf("\nTimer Interrupt......\n");
	printf("Ready Queue size before switching process: %d\n",
			Size(this->ready_queue));
	this->current_process->pcb->state = INTERRUPTED;
	this->switchProcess(this, &PC, TIMER_INT,NULL);
	printf("Ready Queue size after switching process: %d\n",
			Size(this->ready_queue));
	printf("Current Process: %d\n", this->current_process->pcb->pid);

	printf("\nIO Interrupt......\n");
	p = this->current_process;
	this->current_process->pcb->state = INTERRUPTED;
	printf("Ready Queue size before switching process: %d\n",
			Size(this->ready_queue));
	printf("IO Queue size before switching process: %d\n", Size(this->io_queue));
	this->switchProcess(this, &PC, KEYBOARD_SERVICE_REQ,NULL);
	printf("Current Process: %d\n", this->current_process->pcb->pid);
	printf("IO Queue size after switching process: %d\n", Size(this->io_queue));
	printf("Ready Queue size after switching process: %d\n",
			Size(this->ready_queue));

	p = Front(this->io_queue);
	printf("Process %d's status after interrupt is %d \n", p->pcb->pid,
			p->pcb->state);

	q = (ProcessPtr*) getQueue(this->io_queue);
	i = 0;
	while (i < Size(this->io_queue)) {
		printf("%d\n", q[i]->pcb->pid);
		i++;
	}

}

//int main(void) {
//	SchedulerPtr slr = SchedulerConstructor(10);
//	testScheduler(slr);
//
//	return 0;
//}

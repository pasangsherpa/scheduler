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
	scheduler->kb_queue = CreateQueue(max_processes);
	scheduler->mutex = MutexConstructor();

	//Pointers to functions.
	scheduler->destroy = SchedulerDestructor;
	scheduler->setCurrentProcess = setCurrentProcess;
	scheduler->getCurrrentProcess = getCurrentProcess;
	scheduler->addToQueue = addToQueue;
	scheduler->switchProcess = switchProcess;
	scheduler->printQueue = printQueue;
	return scheduler;
}

//Destructor
int SchedulerDestructor(SchedulerPtr this) {
	DisposeQueue(this->ready_queue);
	DisposeQueue(this->io_queue);
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
	this->current_process->pcb->state = RUNNING;
	return NO_ERROR;
}

/**
 * Switch the current running process with the next process in readyqueue and
 * return the new current running process.
 * Assumes all saving context is taken care of before making call to this function.
 */
ProcessPtr switchProcess(SchedulerPtr this, int *PC, int interrupt,
		ProcessPtr prcss) {
	ProcessPtr process = NULL;
	printf("\nServicing Interrupt....\n");
	switch (interrupt) {
	case TIMER_INT:
		printf("Time quanta for process %d is up.\n",
				this->current_process->pcb->pid);
		this->addToQueue(this, this->current_process, this->ready_queue);
		this->current_process->pcb->state = READY;
		this->current_process->pcb->next_step = *PC; //Store the current PC
		this->setCurrentProcess(this);
		printf("Loading next process from Ready Queue.\n");
		printf("Process %d is running\n", this->current_process->pcb->pid);
		*PC = this->current_process->pcb->next_step; //Start where it was left at
		break;

	case VIDEO_SERVICE_REQ:
	case AUDIO_SERVICE_REQ:
	case KEYBOARD_SERVICE_REQ:
		printf("IO Interrupt occured.\n");
		printf("Blocking process %d ......\n", this->current_process->pcb->pid);
		this->addToQueue(this, this->current_process, this->io_queue);
		this->current_process->pcb->state = BLOCKED; //waiting on io
		this->current_process->pcb->next_step = *PC - 1; //Store the current PC
		this->setCurrentProcess(this);
		*PC = this->current_process->pcb->next_step; //Start where it was left at
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

	case MUTEX_LOCK:
		if (!this->mutex->mutex_locked && !IsEmpty(this->mutex->mutex_queue)) {
			this->mutex->lock(this->mutex, true);
			this->current_process->pcb->waiting_on = NOT_WAITING;
			this->mutex->owner = this->current_process;
			this->current_process->pcb->state = READY;
			FrontAndDequeue(this->mutex->mutex_queue);
			this->addToQueue(this, this->current_process, this->ready_queue);
		} else {
			this->current_process->pcb->waiting_on = this->mutex->mutex_id;
			this->current_process->pcb->state = BLOCKED;
			this->addToQueue(this, this->current_process, this->mutex->mutex_queue);
		}
		FrontAndDequeue(this->ready_queue);//the process needs to be removed from the ready queue.
		this->setCurrentProcess(this);

		break;
	case MUTEX_UNLOCK:
		if (this->mutex->owner->pcb->pid == this->current_process->pcb->pid) {
			this->mutex->lock(this->mutex, false);
			if (!IsEmpty(this->mutex->mutex_queue)) {
				process = (ProcessPtr) FrontAndDequeue(this->mutex->mutex_queue);
				this->mutex->owner = process;
				process->pcb->state = READY;
			}
			this->mutex->owner = NULL;
			this->addToQueue(this, process, this->ready_queue);
		}
		break;

	case COND_WAIT:
		break;

	case COND_SIGNAL:
		break;
	default:
		FrontAndDequeue(this->ready_queue);//the process needs to be removed from the ready queue.
		this->setCurrentProcess(this);
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
 * Prints the queue with it's element
 */
void printQueue(SchedulerPtr this, int q_type) {
	char* name;
	Queue Q;
	switch (q_type) {
	case (READY_QUEUE):
		Q = this->ready_queue;
		name = "Ready Queue";
		break;
	case (IO_QUEUE):
		Q = this->io_queue;
		name = "IO Queue";
		break;
	case (MUTEX_QUEUE):
		//		Q = this->mutex->mutex_queue;
		name = "Mutex Queue";
		break;
	default:
		Q = this->ready_queue;
		name = "Ready Queue";
		break;
	}

	ProcessPtr* q = (ProcessPtr*) getQueue(Q);
	int i = 0;
	printf("%s = ", name);
	while (i < Size(Q)) {
		printf("%d:", q[i]->pcb->pid);
		i++;
	}
	printf("\n");
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

	this->addToQueue(this, p1, this->ready_queue);
	this->addToQueue(this, p2, this->ready_queue);
	this->addToQueue(this, p3, this->ready_queue);
	this->addToQueue(this, p4, this->ready_queue);
	this->addToQueue(this, p5, this->ready_queue);

	printQueue(this, READY_QUEUE);

	printf("Ready Queue size before setting current process: %d\n",
			Size(this->ready_queue));

	this->setCurrentProcess(this); //Initialization.
	printQueue(this, READY_QUEUE);
	printf("Ready Queue size after setting current process: %d\n",
			Size(this->ready_queue));

	printf("Current Process: %d\n", this->current_process->pcb->pid);
	ProcessPtr p = this->current_process;
	int PC = 999;
	printf("\nTimer Interrupt......\n");
	printf("Ready Queue size before switching process: %d\n",
			Size(this->ready_queue));
	this->current_process->pcb->state = INTERRUPTED;
	this->switchProcess(this, &PC, TIMER_INT, NULL);
	printf("Ready Queue size after switching process: %d\n",
			Size(this->ready_queue));
	printf("Current Process: %d\n", this->current_process->pcb->pid);

	printf("\nIO Interrupt......\n");
	p = this->current_process;
	this->current_process->pcb->state = INTERRUPTED;
	printf("Ready Queue size before switching process: %d\n",
			Size(this->ready_queue));
	printf("IO Queue size before switching process: %d\n", Size(this->io_queue));
	this->switchProcess(this, &PC, KEYBOARD_SERVICE_REQ, NULL);
	printf("Current Process: %d\n", this->current_process->pcb->pid);
	printf("IO Queue size after switching process: %d\n", Size(this->io_queue));
	printf("Ready Queue size after switching process: %d\n",
			Size(this->ready_queue));

	p = Front(this->io_queue);
	printf("Process %d's status after interrupt is %d \n", p->pcb->pid,
			p->pcb->state);

	printQueue(this, IO_QUEUE);

}

//int main(void) {
//	SchedulerPtr slr = SchedulerConstructor(10);
//	testScheduler(slr);
//
//	return 0;
//}

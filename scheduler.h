/*
 * scheduler.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct scheduler {
	Queue ready_queue;
	ProcessPtr current_process;

	//pointers to functions
	int (*destroy)(struct scheduler*);
	int(*setCurrentProcess)(struct scheduler*);
	ProcessPtr(*switchProcess)(struct scheduler*, int);
	int(*addToReadyQueue)(struct scheduler*, ProcessPtr);
	ProcessPtr(*getCurrrentProcess)(struct scheduler*);

} SchedulerStr, *SchedulerPtr;

SchedulerPtr SchedulerConstructor(int max_processes);
int SchedulerDestructor(SchedulerPtr this);
int setCurrentProcess(SchedulerPtr this);
ProcessPtr switchProcess(SchedulerPtr this, int pc);
int addToReadyQueue(SchedulerPtr this, ProcessPtr process);
ProcessPtr getCurrentProcess(SchedulerPtr this);


#endif /* SCHEDULER_H */

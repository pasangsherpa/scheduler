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

#ifndef QUEUE_H
#include "queue.h"
#endif

#ifndef PROCESS_H
#include "process.h"
#endif

#ifndef MUTEX_H
#include "mutex.h"
#endif

typedef struct scheduler {
	Queue ready_queue;
	Queue io_queue;
	MutexPtr mutex;

	ProcessPtr current_process;

	//pointers to functions
	int (*destroy)(struct scheduler*);
	int(*setCurrentProcess)(struct scheduler*);
	ProcessPtr(*switchProcess)(struct scheduler*, int*, int);
	int(*addToQueue)(struct scheduler*, ProcessPtr, Queue);
	ProcessPtr(*getCurrrentProcess)(struct scheduler*);

} SchedulerStr, *SchedulerPtr;

SchedulerPtr SchedulerConstructor(int max_processes);
int SchedulerDestructor(SchedulerPtr this);
int setCurrentProcess(SchedulerPtr this);
ProcessPtr switchProcess(SchedulerPtr this, int *pc, int interrupt);
int addToQueue(SchedulerPtr this, ProcessPtr process, Queue Q);
ProcessPtr getCurrentProcess(SchedulerPtr this);


#endif /* SCHEDULER_H */

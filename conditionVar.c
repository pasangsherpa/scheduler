/*
 * conditionVar.c
 *
 *  Created on: May 14, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
 
#include "conditionVar.h"

#ifndef PAIR_C
#include "pair.h"
#endif

//Limit of the queue, and the id of this condition variable{NOT_FULL,NOT_EMPTY}
ConditionPtr ConditionConstructor(int limit, int ID){
	ConditionPtr condition = (ConditionPtr) malloc(sizeof(ConditionStr));
	condition->queue = (Queue)CreateQueue(limit);
	condition->id = ID;
	
	return condition;
}
//This method unlocks the process' mutex, adds the process to the condition's queue,
// and upcalls the scheduler.
void condition_wait(ConditionPtr this, MutexPtr mutex, SchedulerPtr scheduler){	
	//CPU saves this process' state before executing this method.	
	MutexLock(mutex, false);
	PairPtr pair = constructPair(scheduler->current_process,mutex);
	Enqueue(pair,this->queue);	
	switchProcess(scheduler, &scheduler->current_process->pcb->next_step, 10, NULL);//upcall to the
												//scheduler(scheduler removes the process
												//from the ready queue)
												//cpu runs a new process.
}

//This method removes the first process from the given condition's queue, 
//and adds it to the process' mutex queue.
void condition_signal(ConditionPtr this){
	if(isEmpty(this->queue) == false){
		PairPtr pair = FrontAndDequeue(this->queue);
		MutexAdd(pair->lock, pair->p);
	}
}

void ConditionDeconstructor(ConditionPtr this){
	DisposeQueue(this->queue);
	free(this);
}

int getConditionID(ConditionPtr this){
	return this->id;
}

	
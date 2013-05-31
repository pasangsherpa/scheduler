/*
 * conditionVar.h
 *
 *  Created on: May 18, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef CONDITION_H
#define CONDITION_H

#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef SCHEDULER_H
#include "scheduler.h"
#endif

#ifndef MUTEX_H
#include "mutex.h"
#endif

 typedef struct{
	int id;
	Queue queue;	
}ConditionStr;
typedef ConditionStr * ConditionPtr;

ConditionPtr ConditionConstructor(int limit, int ID);
void condition_wait(ConditionPtr this, MutexPtr mutex, SchedulerPtr scheduler);
void condition_signal(ConditionPtr this);
void ConditionDeconstructor(ConditionPtr this);
int getConditionID(ConditionPtr this);

#endif
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

 #include <stdio.h>
 #include <stdlib.h>
 #include "global.h"
 #include "queue.h"
#include "mutex.h"
#include "conditionVar.h"

/*
* ConditionVarPtr ConditionVarConstructor()
* Basic Constructor for a Condition Variable object.
*/
ConditionVarPtr ConditionVarConstructor() {
	ConditionVarPtr conditionVar = (ConditionVarPtr) malloc(sizeof(ConditionVar));
	conditionVar -> mutex = (MutexPtr) MutexConstructor();

	conditionVar -> wait_queue = (Queue) CreateQueue(QUEUE_SIZE);

	return conditionVar;
}

void ConditionVarDestructor(ConditionVarPtr this) {
	DisposeQueue(this -> mutex);
	MutexDestructor(this -> mutex);
	free(this);
}
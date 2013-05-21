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

 typedef struct {
 	MutexPtr mutex;
 	Queue wait_queue;
 } ConditionVar, *ConditionVarPtr;

// Constructor
ConditionVarPtr ConditionVarConstructor();

// Destructor
void ConditionVarDestructor(MutexPtr);

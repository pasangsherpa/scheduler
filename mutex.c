/*
 * mutex.c
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

/*
* MutexPtr MutexConstructor()
* Basic Constructor for a Mutex object.
*/
MutexPtr MutexConstructor() {
	MutexPtr mutex = (MutexPtr) malloc(sizeof(Mutex));
	mutex->mutex_locked = false;
	mutex -> mutex_id = -1;
	mutex -> mutex_queue = (Queue) CreateQueue(QUEUE_SIZE);

	//Pointers
	mutex->add = MutexAdd;
	mutex->isLocked = MutexIsLocked;
	mutex->setOwner = setOwner;
	mutex->switchOwner = switchOwner;
	return mutex;
}

void MutexDestructor(MutexPtr this) {
	DisposeQueue(this -> mutex_queue);
	free(this);
}

//Add process to the queue.
void MutexAdd(MutexPtr m, ProcessPtr p) {
	if (!IsFull(m->mutex_queue)) {
		Enqueue(p, m -> mutex_queue);
	}
}

//Lock/Unlock mutex.
void MutexLock(MutexPtr m, bool lock) {
	m->mutex_locked = lock;
}
//Check if mutex is locked
bool MutexIsLocked(MutexPtr m) {
	return m->mutex_locked;
}

//Set the owner of the mutex.
void setOwner(MutexPtr m, ProcessPtr p) {
	if (!m->isLocked) {
		m->owner = p;
	}
}

//Change the owner of the mutex
ProcessPtr switchOwner(MutexPtr m){
	if (!IsEmpty(m->mutex_queue)) {
		m->lock(m, true);
		m->owner = FrontAndDequeue(m->mutex_queue);
		m->owner->pcb->owns = m->mutex_id;
		m->owner->pcb->state = READY;
	}
	return m->owner;
}

//
//int main() {
//		MutexPtr mutex = (MutexPtr) MutexConstructor();
//		int i = 5;
//
//		printf("\ni's address: %d\n", &i);
//		MutexAdd(mutex, &i);
//		return 0;
//}


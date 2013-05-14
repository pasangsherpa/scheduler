/*
 * mutex.c
 *
 *  Created on: May 14, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
* MutexPtr MutexConstructor()
* Basic Constructor for a Mutex object.
*/
MutexPtr MutexConstructor() {
	MutexPtr mutex = (MutexPtr) malloc(sizeof(Mutex));
	mutex -> mutex_ownerID = -1;

	mutex -> mutex_queue = (Queue) CreateQueue(QUEUE_SIZE);

	return mutex;
}

void MutexDestructor(MutexPtr this) {
	DisposeQueue(this -> mutex_queue);
	free(this);
}

void MutexAdd(MutexPtr m, *char id) {
	Enqueue(id, m -> mutex_queue);
}
/*
 * mutex.h
 *
 *  Created on: May 14, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

 typedef struct {
 	int mutex_ownerID;
 	Queue mutex_queue;
 } Mutex, *MutexPtr;

// Constructor
MutexPtr MutexConstructor();

// Destructor
void MutexDestructor(MutexPtr);

// Functions
void MutexAdd(MutexPtr, int);			// Adds an id (pointer) to the mutex queue.
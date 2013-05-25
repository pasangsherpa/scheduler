/*
 * mutex.h
 *
 *  Created on: May 14, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef PROCESS_H
#include "process.h"
#endif

#ifndef GLOBAL_H
#include "global.h"
#endif

typedef struct mutex {
	int mutex_id;
	ProcessPtr owner;
	bool mutex_locked;
	Queue mutex_queue;

	//Pointers to functions
	void(*setOwner)(struct mutex*, ProcessPtr);
	ProcessPtr(*switchOwner)(struct mutex*);
	void(*add)(struct mutex*, ProcessPtr);
	void(*lock)(struct mutex*, bool);
	bool(*isLocked)(struct mutex*);
} Mutex, *MutexPtr;

// Constructor
MutexPtr MutexConstructor();

// Destructor
void MutexDestructor(MutexPtr);

// Functions
void setOwner(MutexPtr mutex, ProcessPtr p);
ProcessPtr switchOwner(MutexPtr mutex);
bool MutexIsLocked(MutexPtr mutex);
void MutexLock(MutexPtr mutex, bool lock);
void MutexAdd(MutexPtr mutex, ProcessPtr);

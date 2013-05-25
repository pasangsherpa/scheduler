/*
 * systemtimer.c
 *
 *  Created on: May 22, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "global.h"
#include "systemtimer.h"


/*
* Basic Constructor for the system timer.
* Initializes the count to the quanta set in global.h.
* Sets the boolean flag to false because the quanta has just started.
* Creates a thread to run the timer.
*/
SysTimerPtr SysTimerConstructor() {
	SysTimerPtr timer = (SysTimerPtr) malloc(sizeof(SysTimerStr));
	timer -> count = QUANTA;
	timer -> quanta_finished = false;

	pthread_create(&timer -> clock_thread, NULL, SysTimerRun, (void *) timer);
	return timer;
}

/*
* Basic Destructor.
* Cancels the thread and frees the memory.
*/
void SysTimerDestructor(SysTimerPtr this) {
	pthread_cancel(this -> clock_thread);
	free(this);
}

void *SysTimerRun(void *args) {
	SysTimerPtr timer = (SysTimerPtr) args;
	if (timer -> count > 0) {		// Quanta not yet finished.
		timer -> count--;
	} else if (timer -> quanta_finished == false) {
		timer -> quanta_finished = true;
	}
}

void STReset(SysTimerPtr timer) {
	timer -> count = QUANTA;
	timer -> quanta_finished = false;
}

bool STisQuantaFinished(SysTimerPtr timer) {
	return timer -> quanta_finished;
}

int main() {
	SysTimerPtr timer = SysTimerConstructor();
}
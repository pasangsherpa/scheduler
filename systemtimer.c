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
* Creates a thread to run the timer.
*/
SysTimerPtr SysTimerConstructor(CPU c, pthread_cond_t condition) {
	SysTimerPtr timer = (SysTimerPtr) malloc(sizeof(SysTimerStr));
	timer -> cpu = c;
	timer -> reset = condition;
	timer -> mutex = PTHREAD_MUTEX_INITIALIZER;

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

	while (true) {
		sleep(QUANTA);
		interruptCPU(timer -> cpu, TIMER_INT, '0');
		pthread_cond_wait(&timer -> reset, &timer -> mutex);
	}
}

/*
int main() {
	pthread_cond_t reset = PTHREAD_COND_INITIALIZER;
	TestCPUPtr cpu = TestCPUConstructor(10, reset);
	SysTimerPtr timer = SysTimerConstructor(cpu, reset);

	int i;
	for (i = 0; i< 9999999999; i++) {}
}
*/

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
#include "systemtimer.h"

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

/*
* Basic Constructor for the system timer.
* Initializes the count to the quanta set in global.h.
* Creates a thread to run the timer.
*/
SysTimerPtr SysTimerConstructor(ICPtr ic, pthread_cond_t condition) {
	SysTimerPtr timer = (SysTimerPtr) malloc(sizeof(SysTimerStr));
	timer -> interruptController = ic;
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
		interruptCPU(timer -> interruptController, TIMER_INT, '0');
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

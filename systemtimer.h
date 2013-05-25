/*
 * systemtimer.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

typedef struct timer {
	int count;								// The countdown variable for the clock.
	bool quanta_finished;
	pthread_t clock_thread;

} SysTimerStr, *SysTimerPtr;


/***************
** Constructor **
****************
* Returns a pointer to the new System Timer and starts the timer.
*/
SysTimerPtr SysTimerConstructor();

/**************
** Destructor **
***************
* The System timer that is pointed to will be destroyed.
*/
void SysTimerDestructor(SysTimerPtr);

/******************************************************************

/*******************
***** Functions *****
*******************/

// Resets the timer to the original Quanta.
void STReset(SysTimerPtr);

// Returns true if the Quanta has been reached.
// To begin again, the timer will then need to be reset.
bool STisQuantaFinished(SysTimerPtr);

// The function that runs the clock
void *SysTimerRun(void *);


#endif /* SYSTEMTIMER_H */

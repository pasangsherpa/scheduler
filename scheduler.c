/*
 * scheduler.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#include "scheduler.h"

SchedulerPtr SchedulerConstructor() {
	SchedulerPtr scheduler = (SchedulerPtr)malloc(sizeof(SchedulerStr));

	return scheduler;
}

/*
 * iodevice.c
 *
 *  Created on: June 6, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

 void IODeviceConstructor(CPUPtr c, pthread_cond_t condition) {
 	SysTimerPtr timer = (SysTimerPtr) malloc(sizeof(SysTimerStr));
 	timer -> cpu = c;
 	timer -> reset = condition;
 	timer -> mutex = PTHREAD_MUTEX_INITIALIZER;

 	pthread_create(&timer -> clock_thread, NULL, SysTimerRun, (void *) timer);
 	return timer;
}
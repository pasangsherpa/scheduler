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

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "global.h"
#include "TestCpu.h"
#include "iodevice.h"

 IODevicePtr IODeviceConstructor(CPUPtr c, pthread_cond_t condition, int type) {
 	IODevicePtr device = (IODevicePtr) malloc(sizeof(IODeviceStr));
 	device -> cpu = c;
 	device -> reset = condition;
 	device -> device_type = type;
 	device -> mutex = PTHREAD_MUTEX_INITIALIZER;

 	pthread_create(&device -> device_thread, NULL, DeviceRun, (void *) device);
 	return device;
}

void IODeviceDestructor(IODevicePtr this) {
	pthread_cancel(this -> device_thread);
	free(this);
}

void *DeviceRun(void *args) {
	IODevicePtr device = (IODevicePtr) args;
	srand(time(NULL));
	float r;

	while (true) {
		pthread_cond_wait(&device -> reset, &device -> mutex);
		/*
		r = (float) rand() %  (float) (IO_QUANTA_UPPER - IO_QUANTA_LOWER);
		r += IO_QUANTA_LOWER;
		printf("\nr: %f", r);

		sleep(r);
		*/
		//interruptCPU(timer -> cpu, TIMER_INT, '0');

	}
}


int main() {
	pthread_cond_t reset = PTHREAD_COND_INITIALIZER;
	CPUPtr cpu = CPUConstructor(10, reset);
	IODevicePtr device = IODeviceConstructor(cpu, reset, IO_AUDIO);

	int i;
	for (i = 0; i< 99999; i++) {}
}
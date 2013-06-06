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
#include "iodevice.h"

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

 IODevicePtr IODeviceConstructor(ICPtr ic, int type) {
 	IODevicePtr device = (IODevicePtr) malloc(sizeof(IODeviceStr));
 	device -> interruptController = ic;
 	device -> device_type = type;

 	pthread_create(&device -> device_thread, NULL, DeviceRun, (void *) device);
 	return device;
}

void IODeviceDestructor(IODevicePtr this) {
	pthread_cancel(this -> device_thread);
	free(this);
}

void *DeviceRun(void *args) {
	IODevicePtr device = (IODevicePtr) args;

	sleep(0.7);
	int interrupt = -1;

	switch(device->device_type) {

		case IO_AUDIO:
			interrupt = AUDIO_SERVICE_COMPLETED;
			break;

		case IO_VIDEO:
			interrupt = VIDEO_SERVICE_COMPLETED;
			break;

		default:
			printf("\nIO Device: (device type not recognized)\n");
			break;
	}

	interruptCPU(device->interruptController, interrupt, '0');
	return 0;
}

/*
int main() {
	ICPtr ic = (ICPtr) ICConstructor();
	IODevicePtr device = IODeviceConstructor(ic, IO_AUDIO);
	printf("\nDevice Started");

	int i;
	for (i = 0; (i < 99999999) &&
					(!isInterruptWaiting(device->interruptController)) ; i++) { }
	printf("\nSignaled!");
}
*/

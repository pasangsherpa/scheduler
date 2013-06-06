/*
 * iodevice.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef IODEVICE_H
#define IODEVICE_H

#ifndef INTERRUPTCONTROLLER_H
#include "interruptController.h"
#endif

#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef QUEUE_H
#include "queue.h"
#endif

#ifndef INTERRUPT_H
#include "interrupt.h"
#endif

typedef struct iodevice {
	int device_type;
	ICPtr interruptController;

	pthread_t device_thread;

} IODeviceStr, *IODevicePtr;


// Constructor
IODevicePtr IODeviceConstructor(ICPtr ic, int type);

// Destructor
void IODeviceDestructor(IODevicePtr this);

void *DeviceRun(void *args);

#endif /* IODEVICE_H */

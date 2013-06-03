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

typedef struct iodevice {
	int device_type;
	CPUPtr cpu;

	pthread_cond_t reset;				// Condition variable CPU uses to signal timer.
	pthread_mutex_t mutex;			// Unused mutex only for signaling.

} IODeviceStr, *IODevicePtr;

#endif /* IODEVICE_H */

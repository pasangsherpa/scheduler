/*
 * pair.h
 *
 *  Created on: May 30, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef PAIR_H
#define PAIR_H

#ifndef PROCESS_H
#include "process.h"
#endif


 typedef struct{
	ProcessPtr p;
	MutexPtr lock;
}PairStr;

typedef PairStr * PairPtr;
PairPtr constructPair(ProcessPtr process, MutexPtr mutex);
void deconstructPair(PairPtr this);

#endif
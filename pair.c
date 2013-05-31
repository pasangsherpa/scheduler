/*
 * pair.c
 *
 *  Created on: May 30, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
 
#include "pair.h"
#include <stdio.h>
#include <stdlib.h>


PairPtr constructPair(ProcessPtr process, MutexPtr mutex){
	PairPtr result = (PairPtr) malloc(sizeof(PairStr));
	result->p = process;
	result->lock = mutex;
	
	return result;
}

void deconstructPair(PairPtr this){
	free(this);
}
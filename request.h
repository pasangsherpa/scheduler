/*
 * request.h
 *
 *  Created on: May 23, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
 
#ifndef REQUEST_H
#define REQUEST_H
 
 typedef struct {
	int process_type;
	int process_step;
} RequestStr, *RequestPtr;

RequestPtr RequestConstructor(int the_type, int the_step_no);
void RequestDeconstructor(RequestPtr this);

int getRequestType(RequestPtr this);
int getRequestStep(RequestPtr this);
int * sort(int * unsorted, int length);

#endif
OBJS = scheduler.o process.o pcb.o queue.o
CC = gcc 
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

build : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o scheduler

scheduler.o: scheduler.c scheduler.h process.h queue.h global.h
	$(CC) $(CFLAGS) scheduler.c

process.o : process.c process.h pcb.h global.h
	$(CC) $(CFLAGS) process.c

pcb.o : pcb.c pcb.h global.h
	$(CC) $(CFLAGS) pcb.c

queue.o : queue.c queue.h
	$(CC) $(CFLAGS) queue.c

clean : 
	-rm -f *.o scheduler

rebuild: clean build

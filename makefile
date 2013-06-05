OBJS = main.o cpu.o scheduler.o systemtimer.o process.o pcb.o mutex.o request.o queue.o
CC = gcc 
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

build : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o scheduler

main.o: main.c systemtimer.h cpu.h 
	$(CC) $(CFLAGS) main.c

cpu.o: cpu.c cpu.h scheduler.h process.h mutex.h request.h queue.h global.h
	$(CC) $(CFLAGS) cpu.c
	
scheduler.o: scheduler.c scheduler.h process.h mutex.h request.h queue.h global.h
	$(CC) $(CFLAGS) scheduler.c

process.o : process.c process.h pcb.h global.h
	$(CC) $(CFLAGS) process.c

pcb.o : pcb.c pcb.h global.h
	$(CC) $(CFLAGS) pcb.c
	
mutex.o : mutex.c mutex.h process.h scheduler.h global.h
	$(CC) $(CFLAGS) mutex.c
	
request.o : request.c request.h 
	$(CC) $(CFLAGS) request.c

systemtimer.o : systemtimer.c systemtimer.h cpu.h
	$(CC) $(CFLAGS) systemtimer.c

queue.o : queue.c queue.h
	$(CC) $(CFLAGS) queue.c

clean : 
	-rm -f *.o scheduler

run :
	./scheduler -p 22 -k 2 -io 3 -pc 3 10

rebuild: clean build

build : scheduler.o process.o pcb.o queue.o
	gcc -W -o scheduler scheduler.o process.o pcb.o queue.o

scheduler.o: scheduler.c scheduler.h process.h queue.h global.h
	gcc -c scheduler.c
process.o : process.c process.h pcb.h global.h
	gcc -c process.c
pcb.o : pcb.c pcb.h global.h
	gcc -c pcb.c
queue.o : queue.c queue.h
	gcc -c queue.c

# mutex.o : mutex.c mutex.h queue.h global.h
#	gcc -c mutex.c

clean : 
	-rm -f *.o scheduler
rebuild: clean build
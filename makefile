build : mutex.o queue.o
	gcc -W -o scheduler mutex.o queue.o
	
mutex.o : mutex.c mutex.h queue.h global.h
	gcc -c mutex.c
queue.o : queue.c queue.h
	gcc -c queue.c
clean : 
	-rm -f *.o scheduler
rebuild: clean build
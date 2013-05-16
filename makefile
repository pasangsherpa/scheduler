build : mutex.o queue.o
	gcc -W -o simulation mutex.o queue.o
	
mutex.o : mutex.c mutex.h queue.h global.h
	gcc -c mutex.c
queue.o : queue.c queue.h
	gcc -c queue.c
clean : 
	-rm -f *.o simulation
rebuild: clean build
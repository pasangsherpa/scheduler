// For testing systemtimer and things using the cpu

typedef struct TestCpu {
        int PC; 				//Program counter
		int max_step_count; 	//Number of steps this CPU will run.
		int next_process;		//the code of the system service this cpu will call
		int next_step;		//when the cpu will call the service
		int process_pid;
		char buffer_data;
		int INT;				//whether INT is asserted
		int IRQ;
		int resume;
		pthread_cond_t reset;

} CPUStr, *CPUPtr;

CPUPtr CPUConstructor(int max_steps, pthread_cond_t);

void interruptCPU(CPUPtr this, int the_IRQ, char the_data);
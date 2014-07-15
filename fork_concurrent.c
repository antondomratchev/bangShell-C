// Example by Sam Birch
// Demonstrates fork() with no waitpid() ...concurrent processing.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char **argv)
{
	int cpid;
	int ppid;
	int i;
	
	//Get the parent's PID
	ppid = getpid();
	printf("Parent[%d]: I'm the only process\n",ppid);
	sleep(1);
	
	//Perform a fork without waiting for the child to terminate (background/concurrent)
	cpid = fork();
	switch(cpid) {
		case -1: // no fork
			fprintf(stderr,"[ERROR] Parent did not fork...exiting now\n");
			return -1;
			break;
		case 0: // fork worked, this is the child
				
			printf("Child[%d]: Newly born child...\n",getpid());
			fflush(stdout);
			
			// some pretend work to show concurrency...
			for (i=0; i<10; i++){
				printf("Child[%d]: Just woke after some sleep...\n",getpid());
				fflush(stdout);
				sleep(1);
			}
			
			printf("Child[%d]: process terminating\n", getpid());
			return 0;
			break;
		default: //fork worked, this is the parent
			// the integer i is in 2 different processes, so they won't stomp on each other			
			printf("Parent[%d] created Child[%d]...\n",getpid(),cpid);
			fflush(stdout);
			
			// pretend work, but less than child to show orphans
			for (i=0; i<3; i++){
				printf("Parent[%d]: Just woke after some sleep...\n",getpid());
				fflush(stdout);
				sleep(1);
			}
			
			printf("Parent[%d]: process terminating...\n", getpid());
			return 0;
			break;
			
	}
	return 0;
}

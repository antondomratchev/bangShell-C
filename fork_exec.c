// Sample code written by Sam Birch to demonstrate how to use
// the exec() family of functions...well as least a couple.
//
// The examples will all have the parent wait for the child 
// to finish...for non-waiting example, see fork_concurrent.c

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

int main(int argc, char **argv)
{
	int cpid, status;
	char input[51];
	// Declare a static arg list for simple execv() example
	char *args[] = {"/usr/bin/cal","-y", NULL}; //Note the NULL terminated array
	
	printf("\n\nFirst example is execv(). This function is well suited for \n");
	printf("situations where your number of args _is not_ known at the \n");
	printf("time of coding.  Press <Enter> to try: ");
	fgets(input,50,stdin); // don't really care about anything but an enter press for this
	cpid = fork();
	switch(cpid){
		case -1:
			printf("Failed fork()...quitting!\n");
			return -1;
			break;
		case 0: // Child
			printf("Fork Successful!\n> Child[%d] will now execv()...\n",getpid());
			fflush(stdout);
			execv("/usr/bin/cal",args); //once printed, the child dies
			break; //should never get here...
		default: //Parent 
			waitpid(cpid,&status,0); // Parent waits for child to die...
			printf("Child died...Parent continuing on...\n\n");
	}
	
	
	
	printf("\n\nSecond example is execl(). This function is well suited for \n");
	printf("situations where your number of args _is_ known at the \n");
	printf("time of coding. \n");
	printf("Enter the absolute path for an ELF binary: ");
	fgets(input,50,stdin); 
	strtok(input,"\n"); //filter new-line
	// We'll assume that the binary exists, but we should check here for the file's existence...
	cpid = fork();
	switch(cpid){
		case -1:
			printf("Failed fork()...quitting!\n");
			return -1;
			break;
		case 0: // Child
			printf("Fork Successful!\n> Child[%d] will now execl(\"readelf\",\"%s\")...\n",getpid(),input);
			fflush(stdout);
			//notice the NULL terminator at the end of the list for execl().
			execl("/usr/bin/readelf","usr/bin/readelf","-r","-h",input,NULL); //once printed, the child dies
			break; //should never get here...
		default: //Parent 
			waitpid(cpid,&status,0); // Parent waits for child to die...
			printf("Child died...Parent continuing on...\n\n");
	}
	
	
	printf("\n\nLast (and fatal) example will call execl() from the parent\n");
	printf("without forking and giving that task to the child.  This is\n");
	printf("well suited for nothing...do not do this! Press <Enter> and\n");
	printf("and note that the mock cleanup code never gets run, since \n");
	printf("the parent essentially commits process suicide: ");
	fgets(input,50,stdin); //just waiting for <enter>
	
	printf("Parent (only process) now calling execl(\"cal\",\"-y\")...bye...\n");
	fflush(stdout);
	execl("/usr/bin/cal","usr/bin/cal","-y", NULL); //once printed, the child dies
	
	// Nothing below here will ever run...if you call exec from your parent
	// in your project #1 shells, the shell will die once the command is executed.
	printf("This is code that will never hit a processor, since the OS overwrote \n");
	printf("the calling parent with the cal program...\n");
	
	printf("hello world\n");
	return 0;
}

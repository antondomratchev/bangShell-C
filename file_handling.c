// Sample code written by Sam Birch to demonstrate
// how to open, read from, write to, and close files

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char **argv)
{
	char filename[21];
	char message[51];
	char readline[51];
	int mypid;
	FILE *fd;
	
	mypid = getpid();
	
	// Let's ask for a file name
	printf("[file_handling:%d] Provide a name for a new file: ", mypid);
	
	// This is a simple demo, but filename sanity checks would be a 
	// good idea in real-life...
	fgets (filename,20,stdin);
	// Getting rid of the line feed at the end of the message...
	strtok(filename,"\n");
	
	printf("[file_handling:%d] Does that file already exist?...",mypid);
	fflush(stdout);
	fd = fopen(filename,"r");
	sleep(1);
	if (fd == NULL) {
		printf("Nope...good to go!\n");
	} else {
		printf("Yup...closing down to protect the innocent!\n");
		return -1;
	}
	
	// If we reached this far, no file or valid fd exists...
	// so let's create the file
	printf("[file_handling:%d] Let's create the file...",mypid);
	fflush(stdout);
	fd = fopen(filename,"a"); // mode is a since we want to write at the end of the file...
	sleep(1);
	if (fd == NULL) {
		printf("No can do...exiting!\n");
		return -2;
	} else {
		printf("Success!\n");
		// Nothing should be in the file yet, so let's write first
		printf("[file_handling:%d] Let's write something in the file...any suggestions (up to 50 characters): ",mypid);
		fgets(message,50,stdin);
		// Getting rid of the line feed at the end of the message...
		strtok(message,"\n");
		fprintf(fd,"%s\n",message);
		fflush(fd);
		fclose(fd);
	}
	
	// Let's try to read the file (on our own without /bin/cat)
	printf("[file_handling:%d] Let's read the file...",mypid);
	fd = fopen(filename,"r"); // mode is r since we want to read from the beginning of the file...
	sleep(1);
	if (fd == NULL) {
		printf("No can do...exiting!\n");
		return -2;
	} else {
		printf("File opened!\n");
		// The loop is for show here since we only allowed a single line of text in the example...
		printf("[file_handling:%d] The contents of file \"%s\" are below:\n",mypid,filename);
		while (fgets(readline, 100, fd) != 0) {
			printf("> %s\n",readline);
		}
		printf("[file_handling:%d] Closing file \"%s\"\n",mypid,filename);		
		fclose(fd);
	}
	
	return 0;
}

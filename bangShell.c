// bangShell created by Anton Domratchev
// 14 Jul 14

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/**
 * Execute /bin/ls in current dirctory
 * @return int
 * @type int
 */
int dir() {
	char *args[] = {"/bin/ls", NULL };
	int cpid, status;
	cpid = fork();
	switch(cpid)
	{
		case -1:
			printf("!#: Failed fork()...quitting!\n");
			return -1;
			break;
		case 0: // Child
			cpid = getpid();
			printf("Child process: %d\n",cpid);
			fflush(stdout);
			execv("/bin/ls", args);
			break; //should never get here...
		default: //Parent
			waitpid(cpid,&status,0); // Parent waits for child to die...
	}
	return 0;
}

/**
 * Create a file using fopen()
 * @params file name
 * @return int
 * @type int
 * @to-do figure out how to fork this method
 */
int create(char *fname) {
	FILE * fp;
	if( access( fname, F_OK ) != -1 )
	{
    	// file exists
		printf("!#:File %s already exists\n",fname);
	}
	else
	{
	    // file doesn't exist
		fp = fopen (fname, "w");
		fflush(stdout);
		printf("!#:File %s is created\n", fname);
		fclose(fp);
	}
	return 0;
}

/**
 * List contents of file using /bin/cat
 * @params file name
 * @return int
 * @type int
 */
int list(char *fname) {
	FILE * fp;
	char *args[] = {"/bin/cat",fname, NULL };
	int cpid, status;
	cpid = fork();
	switch(cpid)
	{
		case -1:
			printf("!#: Failed fork()...quitting!\n");
			return -1;
			break;
		case 0: // Child
			cpid = getpid();
			printf("Child process: %d\n",cpid);
			if( access( fname, F_OK ) == -1 )
			{
		    	// file doesnt exist
				printf("!#:File %s doesn't exist\n",fname);
				kill(cpid, SIGTERM);
			}
			else
			{
				fflush(stdout);
				execv("/bin/cat", args);
				kill(cpid, SIGTERM);
			}
			break;
			default: //Parent
			waitpid(cpid,&status,0); // Parent waits for child to die...
	}
	return 0;
}

/**
 * Update specified file with specific number of lines with specific text
 * @params file name, number of lines, text in "" to update
 * @return int
 * @type int
 */
int update(char *fname, int num, char *text) {
	FILE * fp;
	int cpid, status;
	cpid = fork();
	switch(cpid)
	{
		case -1:
			printf("!#: Failed fork()...quitting!\n");
			return -1;
			break;
		case 0: // Child
			cpid = getpid();
			printf("Child process: %d\n",cpid);
			if( access( fname, F_OK ) == -1 )
			{
		    	// file doesnt exist
				printf("!#:File %s doesn't exist\n",fname);
			}
			else
			{
				fp = fopen (fname, "a");
				int j;
				for(j=0; j<num; j++)
				{
					fprintf(fp, "%s\n",text);
				}
				sleep(strlen(text)/5);
				fflush(stdout);
				fclose(fp);
				kill(cpid, SIGTERM);
			}
			break;
			default: //Parent
			waitpid(cpid,&status,0); // Parent waits for child to die...
	}
	return 0;
}

/**
 * Main shell function
 * @return int
 * @type int
 */
int main(int argc, char **argv)
{
	int pid;
	char uinput[51];
	char original_uinput[51];
	char *command;
	pid = getpid();

	printf("     _   _  _   \n");
	printf("    | |_| || |_ \n");
	printf("    | |_  __  _|\n");
	printf("    | |_| || |_ \n");
	printf("    |_|_  __  _|\n");
	printf("    (_) |_||_|  \n");
	printf("Welcome to Bang Shell!\n");
	printf("======================\n");
	while(1){
		//Prompt
		printf("!#:");
		// Get the user input in a single char array
		fgets(uinput,50,stdin);
		// Set up the tokenizer starting with uInput
		command = strtok(uinput," \t\n");
		if (strcmp(command,"dir")==0)
		{
			printf("Parent process: %d\n",pid);
			dir();
		}
		if (strcmp(command, "create")==0)
		{
			printf("Parent process: %d\n",pid);
			command = strtok(NULL," \t\n");
			create(command);
		}
		if (strcmp(command, "list")==0)
		{
			printf("Parent process: %d\n",pid);
			command = strtok(NULL," \t\n");
			if (command == NULL)
			{
				printf("Please specify which file you would like to list");
			}
			if (command != NULL)
			{
			list(command);
			}
		}
		if (strcmp(command, "update")==0 )
		{
			char *fname;
			int num;
			char *text;
			printf("Parent process: %d\n",pid);

				command = strtok(NULL," \t\n");
				if (command != NULL)
				{
					fname = command;
				}
				command = strtok(NULL," \t\n");
				if (command != NULL)
				{
					num = atoi(command);
				}
				command = strtok(NULL,"\"\t\n");
				if (command != NULL )
				{
					text = command;
				}
				if (fname != NULL && num != 0 && text != NULL)
				{
					update(fname, num, text);
				}


		}
		//info commands
		if (strcmp(command, "bang")==0 || strcmp(command, "!#")==0)
		{
			printf("      _   _  _   \n");
			printf("     | |_| || |_ \n");
			printf("     | |_  __  _|\n");
			printf("     | |_| || |_ \n");
			printf("     |_|_  __  _|\n");
			printf("     (_) |_||_|  \n");
			printf("This is a simple shell\nCreated by\nAnton Domratchev\n\nType help for\navailable commands\n\nSupport for this shell\nis NOT provided!\n\n");
			printf("=======================\n");
		}
		if (strcmp(command, "help")==0)
		{
			printf("create <name>                                                           \n");
			printf("Create a new file in the current directory with the given name.         \n");
			printf("If the file already exists, print an error message on the screen.     \n\n");
			printf("update <name> <number> <text>                                           \n");
			printf("Append lines of text to the named file. In particular, the text string. \n");
			printf("<text> is appended to the end of the file, <number> times.            \n\n");
			printf("list <name>															    \n");
			printf("Display the contents of the named file on the screen.				  \n\n");
			printf("dir 																	\n");
			printf("List the names of all files in the current directory. 					\n");
			printf("halt																	\n");
			printf("Terminate the shell.													\n");
		}
		if (strcmp(command, "halt")==0)
		{
			return 0;
		}
		else
		{
			//printf("!#: Command not found!\n");
		}
	}
	return 0;
}

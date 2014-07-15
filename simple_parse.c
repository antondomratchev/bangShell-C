// Sample by Sam Birch for OS class discussion
// 25 Jun 14

#include <stdio.h>
#include <string.h>

// Example of a function prototype to keep main at the top and implement 
// the function further down.  The next line simply declares the future 
// implementation:
int compare_strings(char *, char *);


int main(int argc, char **argv)
{
	int i;
	char uinput[51];
	char original_uinput[51];
	char *myword;
	
	printf("hello...I'm a simple parser...\n");
	
	//Prompt for several words separated by whitespace...
	printf("Type in a few words: ");
	// Get the user input in a single char array
	fgets(uinput,50,stdin);
	// Copy the original user input to show the difference strtok() makes later...
	strcpy(original_uinput,uinput);
		
	i = 0;
	// Set up the tokenizer starting with uInput
	myword = strtok(uinput," \t\n");
	while (myword != NULL) {
		// Print each word
		printf("Token %d is %s\n", ++i , myword );
		// See if one word matches a specific word
		if (strcmp(myword,"dog")==0) {
			printf("We have a winner...someone typed in \"dog\"\n");
		}
		// Shift to next token...note NULL is used for following
		// calls to strtok().  This modifies uinput...look at the end
		myword = strtok(NULL," \t\n");
	}
	
	printf("uInput was CHANGED by strtok(): \"%s\"\n",uinput);
	compare_strings(original_uinput,uinput);

	return 0;
}





int compare_strings(char *before, char *after) {
	int j;
	printf("          Before    After\n");
	// The below would be a bad practice if we didn't know that the arrays were the same length...
	for (j=0;j<strlen(before);j++) {
		printf("char[%02d]  %03d:%c     %03d:%c\n",j,before[j],before[j],after[j],after[j]);
	}
	return j;
}

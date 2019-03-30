//Perform IO Operations (getchar())
#include <stdio.h> 
//To use the exit() function
#include <stdlib.h> // For exit()
//To use functions from the string library
#include <string.h> 

//command line arguments passed into main
//argc -> number of arguments
//argv -> pointers to the arguments, including the name of the program itself
int main(int argc, char** argv){
	//Check if 3 arguments were passed in. If yes, continue.
	if (argc == 3){
	
	/*Since first argument is the program name,
	the second and third arguments must correspond
	to "from" and "to" respectively*/
	char* from = argv[1];
	char* to = argv[2];

	//Go through all characters to check if from has duplicate chars
	for (unsigned int j = 0; j < strlen(from); j++){
		//k = j + 1 to skip the character being used for comparison
		for (unsigned int k = j + 1; k < strlen(from); k++){
			//If the bytes are equal, print to stderr and exit with code 1
			if (from[j] == from[k]){
				fprintf(stderr, "From contains duplicate bytes. Exiting.");
				exit(1);
			}
		}
	}

	//Check to make sure their lengths are equal
	if (strlen(to) == strlen(from)){
	//Transliterate from STDIN to STDOUT using the arguments passed in
	//getchar returns an int, and gets a byte from stdin stream
	int present = getchar();
	//Until the program reaches EOF (EOF is an int, so type comparison works)
	while (present != EOF){
		//Go through all bytes in from
		for (unsigned int i = 0; i < strlen(from); i++){
			//If input byte equals the ith character in from
			if (present == from[i]){
				//Set input byte to ith character in to
				present = to[i];
				//Break out of the for loop
				break;
			}
		}

		//adds present bte to stdout stream
		putchar(present);
		//Get next byte from stdin
		present = getchar();
	}

	//Return with exit code 0 to indicate success
	exit(0);
	}

	//If not equal, write to stderr and exit with code 1
	else {
		fprintf(stderr, "Arguments must be of same length. Exiting.");
		exit(1);
	}

	}

	//If not, print error to stderr and exit with code 1 (by convention)
	else {
		fprintf(stderr, "Must pass in 3 arguments. Exiting.");
		exit(1);
	}

}
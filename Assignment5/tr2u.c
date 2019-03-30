//Perform IO Operations (getchar())
#include <stdio.h> 
//To use the exit() function
#include <stdlib.h> // For exit()
//To use functions from the string library
#include <string.h> 
//For read and write system functions
#include <unistd.h> 

//command line arguments passed into main
//argc -> number of arguments
//argv -> pointers to the arguments, including the name of the program itself
int main(int argc, char** argv){
	//Check if 3 arguments were passed in
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
	//One byte long (size of a character) to store the unbuffered input
	char present[1];
	//File Descriptor (0) -> standard input
	//Size is 1 byte because we want to take in one character at a time
	ssize_t state = read(0, present, 1);
	//Read returns 0 on reaching end of file, so while greater
	//Returns -1 if error or signal interrupt, so this condition works
	while (state > 0){
		//Go through all bytes in from
		for (unsigned int i = 0; i < strlen(from); i++){
			//If input byte equals the ith character in from
			if (present[0] == from[i]){
				//Set input byte to ith character in to
				present[0] = to[i];
				//Break out of the for loop
				break;
			}
		}

		//File Descriptor (1) -> standard output
		//Size is 1 byte because we want to output one character at a time
		write(1, present, 1);
		//Update status using read() functin
		state = read(0, present, 1);
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
	else{
		fprintf(stderr, "Must pass in 3 arguments. Exiting.");
		exit(1);
	}

}

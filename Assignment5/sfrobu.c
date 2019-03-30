#include <stdio.h>
#include <ctype.h> //toupper, or tolower. 
#include <string.h> //strcmp
#include <unistd.h> //fstat
#include <stdlib.h> //malloc, free, realloc
#include <sys/stat.h> //fstat

//Check for IO error
int IOErrorCheck(int readStatus) {
	//Returns 1 if error, else returns false
	//-1 indicates error, while 0 or positive values indicate success
	if (readStatus < 0) {
		fprintf(stderr, "I/O Error! Exiting.");
		//Failure,so return false
		return 0;
	}
	//Successfully read input, return true
	return 1;
}

//Defining frobcmp
int frobcmp(char const* a, char const* b) {
	/*memfrob transforms (frobnicates) each byte of the data
	structure at mem, which is length bytes long, by bitwise
	exclusive oring it with binary 00101010. It does the
	transformation in place and its return value is always mem.*/
	/*Note that memfrob a second time on the same data structure
	returns it to its original state.*/
	// 00101010 in hex is 0x2A
	//Keep looping, and increment a and b every loop
	for (;;a++,b++) {
		while (*a == '\0')
			//Allows for null bytes
			a++;
		while (*b == '\0')
			//Allows for null bytes
			b++;
		if (*a == ' ' && *b == ' ') {
			//if both words end, then they're the same.
			return 0;
		}
		else if (*b == ' ' || (*b ^ 0x2A) < (*a ^ 0x2A)) {
			//b ends before a, the b < a
			return 1;
		}
		else if (*a == ' ' || (*a ^ 0x2A) < (*b ^ 0x2A)) {
			//a ends before b, or a < b
			return -1;
		}
	}
}

//Compare input chars
int comp(const void* val1, const void* val2) {
	return frobcmp(*(const char**) val1, *(const char**)val2);
}

//Main
int main(int argc, char** argv) {
	//Check if number of args either 1 or 2
	size_t flag = 0;
	const char* Flag = argv[1]; 
	if (argc <= 2){
		if (argc == 2 && (strcmp(Flag,"-f") == 0)){
			flag = 1;
		}
		//If two arguments but not -f, print to stderr and exit with code 0
		if (argc == 2 && (strcmp(Flag,"-f") != 0)){
			fprintf(stderr, "Second argument passed was not -f.");
			exit(1);
		}
	}
	else if (argc > 2) {
		/*If more than 2 arguments (program name + -f argument),
		print to stderr and return with exit code 1*/
		fprintf(stderr, "Too many arguments. Only valid argument is -f");
		exit(1);
	}

	//By this point, we know the argument is -f if two arguments are passed in

	char* word = NULL; //Pointer to each word
	char** wordList = NULL; //Stores pointers to each of the words in the wordList
	int n_word = 0; //Counter for word
	unsigned int n_wordList = 0; //Counter for worldList
	char* buffer; //Create filebuffer
	int buffCount = 0, buffMax = 0; //Keep track of max and current size
	//Signed int to capture value returned by the read function (>0, 0 or -1)
	//Can also be ssize_t, but int allows for wider value range
	int status;

	//The buf argument is a pointer to a stat structure, therefore we must create a buff stat struct
	struct stat fileData;
	//Returns 0 if successful, -1 if failure
	if (fstat(0, &fileData) == -1){
		//Print error and return with exit code 1 
		fprintf(stderr, "Fstat Error.");
		exit(1);
	}

	//POSIX Macro S_ISREG used to understand values in the fileData stat struct
	//True if fileData is a regular file, under protection
	if (S_ISREG(fileData.st_mode)){
		//Allocate enough space. st_size returns size in bytes.
		//Since sizeof(char) = 1 byte, this is effectively allocating enough space for the file
		buffer = (char*)malloc(fileData.st_size * sizeof(char));
		//If failure to allocate, print to stderr and exit with code 1
		if (buffer == NULL){
			fprintf(stderr, "Memory Allocation Error.");
			exit(1);
		}
		//If error reading, returns -1
		//Read into fileBuffer from stdin (the file) everything in the file
		if (read(0, buffer, fileData.st_size) == -1){
			//Write to stderr and return with exit code 1
			fprintf(stderr, "Read Error.");
			exit(1);
		}
		//Set MaxSize counter to the size of the file.
		buffMax = fileData.st_size;
	}

	//Otherwise just use stdin from the terminal

	//Allocate space for the word
	word = (char*)malloc(sizeof(char));
	//Check if malloc fails, exit
	//Write to stderr and return with exit code 1
	if (word == NULL) {
		fprintf(stderr, "Malloc Error. Exiting.");
		exit(1);
	}
	
	//Ensure there is no read error
	if (buffCount >= buffMax){
		//Update value of status based on return value from read function
		//Read one byte into the word buffer
		status = read(0, word, 1);
		//If IO error, free memory and exit
		if (!IOErrorCheck(status)) {
			free(word);
			fprintf(stderr, "IOError. Exiting");
			exit(1);
		}
	}
	else {
		word[n_word] = buffer[buffCount++];
		status = 1;
	}

	//While no IOError and words stil left in input stream
	while (status > 0) {
		//Create a next character
		char next;

		//If error reading character, run check through IOErrorCheck
		if (buffCount >= buffMax) {
			status = read(0, &(next), 1);
			if (!IOErrorCheck(status)){
				free(word); //Free current word
				for (unsigned int i = 0; i < n_wordList; i++) {
					//Free all elements from word list until this point
					free(wordList[i]);
				}
				//Free the wordList pointer itself to avoid mem leak
				free(wordList);
				fprintf(stderr, "IO Error.");
				//Exit with code 1
				exit(1);
			}
		}
		else {
			//Update next
			next = buffer[buffCount++];
		}

		//If space
		if (word[n_word] == ' ') {
			//Reallocate space for wordList + 1 
			char **temp = realloc(wordList, n_wordList*(sizeof(char*)) + sizeof(char*));
			//If this reallocation results in a failure, free memory
			if (temp == NULL) {
				free(word);
				for (unsigned int i = 0; i < n_wordList; i++) {
					free(wordList[i]);
				}
				free(wordList);
				fprintf(stderr, "Reallocation Error.");
				exit(1);
			}
			//Else, realloc successful, so
			else {
				wordList = temp; //Point to the new reallocated memory space with the old content
				wordList[n_wordList] = word; //Last element of wordList must point to word
				word = (char*)malloc(sizeof(char)); //Space for one character
				n_wordList++; //Incremenet size of wordList
				n_word = -1; //Decrement so n_word++ makes it 0
			}
			//If reaches EOF (returns 0), then break out completely
			if (status == 0) {
				break;
			}
			//Skip through spaces until it reaches a character
			while (next == ' ') {
				//Check for IOError, free memory and exit with code 1 if necessary
				if (buffCount >= buffMax){
					status = read(0, &(next), 1);
					if (!IOErrorCheck(status)){
						free(word);
						for (unsigned int i = 0; i < n_wordList; i++) {
							free(wordList[i]);
						}
						free(wordList);
						exit(1);
					}
				}
				else {
					//Update next to next element in fileBuffer
					next = buffer[buffCount++];
				}
			}
		}
		//If reach EOF, update next and set status to 1
		else if (status == 0) {
			next = ' ';
			status = 1;
		}
		
		//Go to next character. Increase char count.
		n_word++; 
		//Reallocate space for new word
		char *temp = realloc(word, n_word*(sizeof(char*)) + sizeof(char*));
		//If realloc fails, indicate error and exit
		if (temp == NULL) {
			//Free memory, print to stderr and return with error code 1
			free(word);
			for (unsigned int i = 0; i < n_wordList; i++) {
				free(wordList[i]);
			}
			free(wordList);
			fprintf(stderr, "Realloc error.");
			exit(1);
		}
		else {
			//Else set word to temp
			//Change word[last element] to the next character 
			word = temp;
			word[n_word] = next;
		}
	}

	//if flagSet, set everything to upper case and run sort
	if (flag) {
		for (size_t i = 0; i < n_wordList; i++) {
			//Convert to lower/upper and pass into qsort
		}
		//Sort using qsort
		qsort(wordList, n_wordList, sizeof(char*), comp);
	}
	else {
		//Sort using qsort
		qsort(wordList, n_wordList, sizeof(char*), comp);
	}

	//Output elemets of word array
	for (unsigned int i = 0; i < n_wordList; i++) {
		for (unsigned int j = 0; wordList[i][j] != ' '; j++) {
			//Write to stdout
			write(1, &wordList[i][j], 1);
		}
		//Write to stdout
		write(1, " ", 1);
	}

	//Free remaining memory
	free(word);
	for (unsigned int i = 0; i < n_wordList; i++) {
		free(wordList[i]);
	}
	free(wordList);
	//Return 0 to indicate success
	exit(0);
}
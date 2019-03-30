//To read I/O 
#include <stdio.h>
//malloc, realloc, free, qsort
#include <stdlib.h>

//Check for IO error
int IOErrorCheck() {
	//ferror tests the error indicator for the given stream
	//Returns 1 if error, else returns 0
	if (ferror(stdin)) {
		fprintf(stderr, "I/O Error! Exiting.");
		//Failure,so return 0
		return 0;
	}
	//Successfully read input, return 1
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
	for (;; a++, b++) {
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
int main() {
	char* word = NULL; //Pointer to each word
	char** wordList = NULL; //Stores pointers to each of the words in the wordList
	int n_word = 0; //Number of letters in word
	unsigned int n_wordList = 0; //Number of words in wordList

	word = (char*)malloc(sizeof(char)); //Allocate memory
	//Check if malloc fails, exit
	if (word == NULL) {
		fprintf(stderr, "Error Allocating Memory. Exiting.");
		exit(1);
	}

	//Returns next char from stdin
	word[n_word] = getchar();
	//If IO error, free memory and exit
	if (!IOErrorCheck()) {
		free(word);
		exit(1);
	}

	//Create character so that you can get the next character
	char next;

	//While no IOError and words stil left in input stream
	while (IOErrorCheck() && word[n_word] != EOF) {
		next = getchar();
		//If error reading character
		if (!IOErrorCheck()) {
			free(word); //Free current word
			for (unsigned int i = 0; i < n_wordList; i++) {
				//Free all elements from word list until this point
				free(wordList[i]);
			}
			//Free the wordList pointer itself to avoid mem leak
			free(wordList);
			fprintf(stderr, "IO Error.");
			//Exit
			exit(1);
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
			//If next is EOF, then break of out the while loop
			if (next == EOF) {
				break;
			}
			//Skip through spaces until it reaches a character
			while (next == ' ') {
				//next = next character
				next = getchar();
				//If error reading input, exit
				if (!IOErrorCheck()) {
					fprintf(stderr, "IO Error.");
					exit(1);
				}
			}
		}
		//If next char is EOF, then set next to ' '
		else if (next == EOF) {
			next = ' ';
		}
		n_word++; //Go to next character. Increase char count.
		//Reallocate space for new word
		char *temp = realloc(word, n_word*(sizeof(char*)) + sizeof(char*));
		//If realloc fails, indicate error and exit
		if (temp == NULL) {
			//Free memory
			free(word);
			for (unsigned int i = 0; i < n_wordList; i++) {
				free(wordList[i]);
			}
			free(wordList);
			fprintf(stderr, "Realloc error.");
			exit(1);
		}
		//Else set word to temp
		//Change word[last element] to the next character 
		else {
			word = temp;
			word[n_word] = next;
		}
	}

	//Sort using qsort
	qsort(wordList, n_wordList, sizeof(char*), comp);

	//Output elemets of word array
	for (unsigned int i = 0; i < n_wordList; i++) {
		for (unsigned int j = 0; wordList[i][j] != ' '; j++) {
			printf("%c", wordList[i][j]);
		}
		printf(" ");
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
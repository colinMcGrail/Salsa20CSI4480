#include "main.h"
#include "libsalsa.h"
#include "inputManagement.h"

int main(int argc, char* argv[]){
	
	int i;

	FILE* keypath;
	FILE* targetpath;
	FILE* outputpath;
	size_t length;
	char mode;
	unsigned long int counter = 0;

	INT32* key;
	INT32* cipher;
	BYTE* nextwords; /* DATATYPES DEFINED IN LIBSALSA.H */
			    /* WE'RE GOING FOR PURE ANSI C HERE, SO NO STDINT.H */

	key = malloc(32);
	nextwords = malloc(64);
	cipher = malloc(64);

	if(argc != 4){
		printf(ERRORMSG_INPUT);
		return 0;
	}

	mode = getMode(argv[1]);

	if(mode == '\0'){
		printf(ERRORMSG_INPUT);
		return 0;
	}
	
	keypath = fopen(argv[2], "r");
	targetpath = fopen(argv[3], "r");
	if(keypath == NULL || targetpath == NULL){
		printf(ERRORMSG_INPUT);
		return 0;
	}

	outputpath = openOutput(argv[3], mode);

	length = fread(key, 32, 1, keypath);

	if(length != 1){
		printf(ERRORMSG_KEY);
		return 0;
	}

	fclose(keypath);

mainloop:

	length = fread(nextwords, 1, 64, targetpath);

	constructArray(cipher, key, counter);

	transformArray(cipher);

	encryptAndWrite(cipher, nextwords, outputpath, length);

	++counter;

	if(length == 64)
		goto mainloop;

	return 1;
}

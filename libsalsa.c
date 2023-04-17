#include "libsalsa.h"

void constructArray(INT32* cipher, INT32* key, unsigned long int counter){

	srand(counter);
	
	cipher[0] = PREDEF1;
	cipher[1] = key[0];
	cipher[2] = key[1];
	cipher[3] = key[2];
	cipher[4] = key[3];
	cipher[5] = PREDEF2;
	cipher[6] = rand();
	cipher[7] = rand();
	cipher[8] = rand();
	cipher[9] = rand();
	cipher[10] = PREDEF3;
	cipher[11] = key[4];
	cipher[12] = key[5];
	cipher[13] = key[6];
	cipher[14] = key[7];
	cipher[15] = PREDEF4;

}
static INT32 leftRotate(INT32 value, int distance){

	return((value << distance) | (value >> (32 - distance)));

}
static void quarterround(INT32* w, INT32* x, INT32* y, INT32* z){
	*x = *x ^ (leftRotate((*w + *z), 7));
	*y = *y ^ (leftRotate((*x + *w), 9));
	*z = *z ^ (leftRotate((*y + *x), 13));
	*w = *w ^ (leftRotate((*z + *y), 18));
}
static void doubleround(INT32* cipher){
	quarterround(&cipher[0], &cipher[4], &cipher[8], &cipher[12]);
	quarterround(&cipher[5], &cipher[9], &cipher[13], &cipher[1]);
	quarterround(&cipher[10], &cipher[14], &cipher[2], &cipher[6]);
	quarterround(&cipher[15], &cipher[3], &cipher[7], &cipher[11]);	

	quarterround(&cipher[0], &cipher[1], &cipher[2], &cipher[3]);
	quarterround(&cipher[5], &cipher[6], &cipher[7], &cipher[4]);
	quarterround(&cipher[10], &cipher[11], &cipher[8], &cipher[9]);
	quarterround(&cipher[15], &cipher[12], &cipher[13], &cipher[14]);
}
void transformArray(INT32* cipher){

	INT32* copy;

	int i;

	copy = malloc(64);

	for(i = 0; i < 16; i++){
		copy[i] = cipher[i];
	}

	for(i = 0; i < 10; i++){
		doubleround(cipher);
	}

	for(i = 0; i < 16; i++){
		cipher[i] = cipher[i] + copy[i];
	}

	free(copy);


}

void encryptAndWrite(INT32* cipher, BYTE* nextwords, FILE* outputpath, size_t length){
	BYTE* choppedcopy;
	int i;
	BYTE* temp;

	choppedcopy = malloc(64);
	memcpy(choppedcopy, cipher, 64);

	for(i = 0; i < length; i++){
		*temp = choppedcopy[i] ^ nextwords[i];
		fwrite(temp, 1, 1, outputpath);
	}

}

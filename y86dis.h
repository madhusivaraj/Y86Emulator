/*Madhumitha Sivaraj
PA3 Extra Credit #1 - Disassembler*/
#ifndef Y86DIS_H_
#define Y86DIS_H_

union cbyte {
	char byte[4]; int val;
};

char* convertHexToBin(char);
long convertBinToDec(char*);
int convertHexToDec(char*);
char* store(char*);
int fetch(char*, int);
char* append (char*, char);

#endif

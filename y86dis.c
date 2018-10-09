/*Madhumitha Sivaraj
PA3 Extra Credit #1 - Disassembler*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "y86dis.h"

char* convertHexToBin(char hex){ //convert hex to binary (taken from pa1)
	char* binary=""; //return value
	switch(hex){ //set binary values of hex values 0-9, A-F (case insensitive)
		case '\0': break;
		case '0': binary="0000"; break;
		case '1': binary="0001"; break;
		case '2': binary="0010"; break;
		case '3': binary="0011"; break;
		case '4': binary="0100"; break;
		case '5': binary="0101"; break;
		case '6': binary="0110"; break;
		case '7':	binary="0111"; break;
		case '8': binary="1000"; break;
		case '9': binary="1001"; break;
		case 'A': case 'a': binary="1010"; break;
		case 'B': case 'b': binary="1011"; break;
		case 'C': case 'c': binary="1100"; break;
		case 'D': case 'd': binary="1101"; break;
		case 'E': case 'e': binary="1110"; break;
		case 'F': case 'f': binary="1111"; break;
		default: fprintf(stderr, "%s\n", "ERROR: Invalid entry. Hex characters are 0-9, A-F (case insensitive)."); return 0;
	}
	return binary; //return hex value
}
long convertBinToDec(char* binary){ //convert binary to dec (taken from pa1)
	long decimal=0; //return value
	int exp=strlen(binary)-1;
	int i=0;
	while(binary[i]!='\0'){
		long dec2=binary[i]-'0';
		decimal+=dec2*(pow(2, exp)); //use math library pow function
		i++; exp--;
	}
	return decimal; //return value
}
int convertHexToDec(char* hex){ //convert hex to dec (taken from pa1)
	int decimal=0; //set result
	int length=strlen(hex);
	char* bASCII=(char *) malloc((4*length+1)*sizeof(char));
	int i=0, j=0;
	while(i<(4*length+1)){
		bASCII[i]='\0';
		i++;
	}
	while(j<length){
		strcat(bASCII, convertHexToBin(hex[j]));
		j++;
	}
	decimal=convertBinToDec(bASCII);
	free(bASCII);
	return decimal;
}
char* store(char* ch){ //store result
	char* result=(char*)(malloc((strlen(ch)+1)*sizeof(char))); //set result using malloc and char
	strcpy(result, ch); //call strcpy function
	return result; //return result
}
int fetch(char* ch, int ptr){ //fetch cycle
	char* data=(char *) malloc(3*sizeof(char)); //use 3 bc hex numbers; store in data
	data[0]=ch[ptr]; //set index 0 to val at ptr index of ch
	data[1]=ch[ptr+1]; //set index 1 to val at ptr+1 index of ch
	data[2]='\0'; //set index 2 to 0; just for space
	int result=convertHexToDec(data); //use convertHexToDec function on data and store in result
	free(data); //free what has been malloced
	return result; //return
}
char* append (char* ch, char value){
	int length=strlen(ch)+2; //length of char
	char* result=(char *)calloc(length, sizeof(char));
	strcpy(result, ch);free(ch);
	result[length-2]=value; //set index of length-2 in result to value
	result[length-1]='\0'; //set the next index to '\0' to reserve space
	return result;
}
int main(int argc, char**argv){//main method{
	const char *reg[8]={"%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"};
	if(argc!=2){ //when less than or more than 2 args
		fprintf(stderr, "%s\n", "ERROR: Number of inputs entered was either greater or less than 2. Valid input is $ ./y86dis <filename>");
		return 1; //failure
	}else if(argc==2){
		if(strcmp(argv[1], "-h")==0){ //usage message
			 fprintf(stderr, "%s\n", "Usage: ./y86dis <y86 file name>");
			 return 1; //failure
		 }
		char* input=strstr(argv[1],".y86"); //checking for .y86 file ending
		 if(input==NULL || strlen(input)!=4 || strlen(argv[1])<5){ //error check to see if file is of .y86 type
			 fprintf(stderr, "%s\n", "ERROR: Invalid file. Must be a .y86 file. Please try again");
			 return 1; //failure
		 }
		 if(strcmp(argv[1], "prog1.y86")!=0){ //only valid files are prog1, prog2 and prog3
			 if(strcmp(argv[1], "prog2.y86")!=0){
				  if(strcmp(argv[1], "prog3.y86")!=0){
		 			 fprintf(stderr, "%s\n", "ERROR: This file does not exist in this directory. Only valid files are prog1.y86, prog2.y86, and prog3.y86. Please try one of these options..");
		 			 return 1; //failure
				 }
			 }
 		 }
		input=(char *)malloc(sizeof(argv[1]));
		strcpy(input, argv[1]);
		if(strlen(input)<5)	{ //if less than 5 - error
				fprintf(stderr, "%s\n", "ERROR: Invalid file. Must be a .y86 file. Usage: ./y86dis <y86 file name>");
				return 1; //failure
		}
		int i=0,j=0;
		while(input[i]!='\0'){ //while not '\0'
			if(input[i]=='.') break;
			i++;
		}
		FILE* filename=fopen(input, "r\0");
		char c;
		char* decode=(char *) calloc(1, sizeof(char));
		decode[0]='\0';
		do{
			c=fgetc(filename);
			if(feof(filename)) break;
			decode=append(decode, c);
		}while(1);
		fclose(filename);
		char* duplicate=store(decode);
		char* token=strtok(duplicate,"\n\t\r");
		char* instr, *programcounter;
		union cbyte convchar;
		int b1, b2, value;
		int x=0;
		do{
			if(strcmp(token, ".text")==0){
				if(x==0) x++;
				else{
					fprintf(stderr, "%s\n", "ERROR");
					return 1; //failure
				}
				programcounter=store(strtok (NULL, "\n\t\r"));
				instr=store(strtok(NULL, "\n\t\r"));
			}
			token=strtok(NULL, "\n\t\r");
			if(token==NULL) break;
		}while (1);
		free(duplicate);
		unsigned char memory[strlen(instr)/2];
		i=0; j=0;
		while(j<strlen(instr)){
			memory[i]=fetch(instr, j);
		 	i++; j+=2;
		}
		i=0, j=0;
		while(i<strlen(instr)/2){
			switch (memory[i]){ //cases
		 			case 0x00: printf("nop\n");	i++; j=1; break;
		 			case 0x10: printf("halt\n"); i++; j=1; break;
		 			case 0x20: b1=(memory[i+1] & 0xf0)>>4; 	b2=(memory[i+1] & 0x0f);
		 												printf("rrmovl\t%s\t%s\n", reg[b1], reg[b2]); i+=2; j=2; break;
		 			case 0x30: b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
														convchar.byte[2]=memory[i+4]; convchar.byte[3]=memory[i+5];
														printf("irmovl\t$%0x\t%s\n", convchar.val, reg[b2]); j=6;	i+=6; break;
		 			case 0x40:	b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														convchar.byte[0]=memory[i+2];	convchar.byte[1]=memory[i+3];
														convchar.byte[2]=memory[i+4]; convchar.byte[3]=memory[i+5];
														value=convchar.val;	printf("rmmovl\t%s\t%d(%s)\n", reg[b1], value, reg[b2]);
														j=6; 	i+=6;	break;
		 			case 0x50: b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
														convchar.byte[2]=memory[i+4]; convchar.byte[3]=memory[i+5];
														value=convchar.val; 	printf("mrmovl\t%d(%s)\t%s\n", value, reg[b2], reg[b1]);
														j=6; i+=6; break;
		 			case 0x60: b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														printf("addl\t%s\t%s\n", reg[b1], reg[b2]); j=2; i+=2; break;
		 			case 0x61:	b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
															printf("subl\t%s\t%s\n", reg[b1], reg[b2]);	j=2; i+=2; break;
		 			case 0x62: b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														printf("andl\t%s\t%s\n", reg[b1], reg[b2]); j=2;	i+=2; break;
		 			case 0x63: b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														printf("xorl\t%s\t%s\n", reg[b1], reg[b2]);	j=2; i+=2; break;
		 			case 0x64:	b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														printf("mull\t%s\t%s\n", reg[b1], reg[b2]); j=2;	i+=2; break;
		 			case 0x65: 	b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
														printf("cmpl\t%s\t%s\n", reg[b1], reg[b2]);	j=2;	i+=2; break;
		 			case 0x70: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val;	printf("jmp\t$0x%x\n", value);	j=5; i+=5;	break;
		 			case 0x71: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
		 												value=convchar.val; printf("jle\t$0x%x\n", value);	j=5; i+=5;	break;
		 			case 0x72: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val;	printf("jl\t$0x%x\n", value);j=5; i+=5;	break;
		 			case 0x73: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val; printf("je\t$0x%x\n", value);j=5; i+=5;	break;
		 			case 0x74: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val;	printf("jne\t$0x%x\n", value); j=5; i+=5;	break;
		 			case 0x75: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
		 												value=convchar.val;	printf("jge\t$0x%x\n", value);  j=5; i+=5;	break;
		 			case 0x76: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val; printf("jg\t$0x%x\n", value); j=5; i+=5;	break;
		 			case 0x80: convchar.byte[0]=memory[i+1]; convchar.byte[1]=memory[i+2];
		 												convchar.byte[2]=memory[i+3]; convchar.byte[3]=memory[i+4];
														value=convchar.val;printf("call\t$0x%x\n", value);  j=5; i+=5;	break;
		 			case 0x90:	printf("ret\n"); 	j=1;	i+=1; break;
		 			case 0xA0: b1=(memory[i+1] & 0xf0)>>4; printf("pushl\t%s\n", reg[b1]); j=2; i+=2; break;
		 			case 0xB0:b1=(memory[i+1] & 0xf0)>>4; printf("popl\t%s\n", reg[b1]); j=2; i+=2; break;
		 			case 0xC0: b1=(memory[i+1] & 0xf0)>>4;	convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
		 													convchar.byte[2]=memory[i+4]; convchar.byte[3]=memory[i+5]; value=convchar.val;
															printf("readb\t%d(%s)\n", value, reg[b1]);	j=6;	i+=6; break;
		 			case 0xC1: b1=(memory[i+1] & 0xf0)>>4;
											 				convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
											 				convchar.byte[2]=memory[i+4];	convchar.byte[3]=memory[i+5];
															value=convchar.val; printf("readl\t%d(%s)\n", value, reg[b1]); j=6;	i+=6; break;
		 			case 0xD0: b1=(memory[i+1] & 0xf0)>>4;
															convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
															convchar.byte[2]=memory[i+4];	convchar.byte[3]=memory[i+5];
		 												value=convchar.val; printf("writeb\t%d(%s)\n", value, reg[b1]);	j=6;	i+=6; break;
		 			case 0xD1: b1=(memory[i+1] & 0xf0)>>4;
														convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
														convchar.byte[2]=memory[i+4];	convchar.byte[3]=memory[i+5];
														value=convchar.val; printf("writel\t%d(%s)\n", value, reg[b1]);	j=6;	i+=6; break;
		 			case 0xE0:	b1=(memory[i+1] & 0xf0)>>4; b2=(memory[i+1] & 0x0f);
															convchar.byte[0]=memory[i+2]; convchar.byte[1]=memory[i+3];
															convchar.byte[2]=memory[i+4];	convchar.byte[3]=memory[i+5];
									 						value=convchar.val; printf("movsbl\t%d(%s)\t%s\n", value, reg[b2], reg[b1]); 	j=6;	i+=6; break;
		 			default: break;
					}
		 		j++;
		 	}
			if(programcounter>0){
				return 0;
			}
		 	free(input);
		 	free(decode);
		 	return 0;
		}
		return 0;
}

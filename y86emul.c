#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "y86emul.h"

void nop(int i, cpu* c){ //method 1
    c->pc+=i; //used for incremental purposes
    if(c->pc>=c->size){ //when pc>size -- error!
        fprintf(stderr, "%s\n", "ERROR: Please try again.");
    }
}
void halt(int i){ //stops execution cycle
  int instr=i; //set instr equal to parameter
  if(instr==-1){ // when instr is -1, STOP program
    exit(0);
  }
}
char* readB(char* file){ //reads a single byte from the terminal into memory
    FILE* filename=fopen(file, "rb"); //set variables
    char* tmp=0;
    long length=0;
    if(filename){ //when filename is valid
        fseek(filename, 0, SEEK_END);
        length=ftell(filename); //length of file name found
        fseek(filename, 0, SEEK_SET);
        tmp=malloc(length); //store length in tmp
        if(tmp){ //if tmp
          fread(tmp, 1, length, filename);
        }
        fclose(filename);
    }else{ //else invalid filename results in error message
        fprintf(stderr, "%s\n", "ERROR: Invalid filename. This file does not exist in this directory. Please try again.");
        exit(0);
    }
    tmp[length]='\0'; //set index length of tmp to null.
    if(!tmp){ //when temp is not valid, error message for invalid file shows up
        fprintf(stderr, "%s\n", "ERROR: Invalid file. Please try again.");
        exit(0);
    }else{ //else, tmp is returned
        return tmp;
    }return file;
}
int checkRegister(int reg){ //ensure register is not greater than 7
	if(reg>7){ //when greater than 7
		fprintf(stderr, "%s\n", "ERROR: Overflow. Register value cannot be over 7.");
		return 1; //failure
	}else if(reg<0){ //when less than 0
    fprintf(stderr, "%s\n", "ERROR: Negative numbers are invalid inputs. Try again with a number between 0-12.");
		return 1; //failure
  }
	return reg;
}
void readX(cpu* c){
  int operator=mem[c->pc].phase.phase1;
  if(operator<0 || operator>1){
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  nop(1, c);
  convchar shift;
  if(mem[c->pc].phase.phase1!=15){
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  checkRegister(mem[c->pc].phase.phase2);
  int destination=c->reg[mem[c->pc].phase.phase2];
  nop(1, c);
  shift.typeCh.b1=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b2=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b3=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b4=mem[c->pc].cbyte; nop(1, c);
  destination += shift.typeInt;
  if(destination<0 || destination>=c->size){
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
    exit(0);
  }
  if(operator==0){
    mem[destination].cbyte=getchar();
    if(mem[destination].cbyte == -1){
      c->ZF=1;
    }else{
      c->ZF=0;
    }
  }else {
    if(destination+4<0 || destination+4>=c->size){
      fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
      exit(0);
    }
    if(EOF == scanf("%d", &shift.typeInt)){
      c->ZF=1;
    }else{
      c->ZF=0;
    }
    mem[destination+3].cbyte=shift.typeCh.b4;
    mem[destination+2].cbyte=shift.typeCh.b3;
    mem[destination+1].cbyte=shift.typeCh.b2;
    mem[destination].cbyte=shift.typeCh.b1;
  }
}
void writeX(cpu* c){
  int operator=mem[c->pc].phase.phase1;
  if(operator<0||operator>1){
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  convchar shift;
  nop(1, c);
  if(mem[c->pc].phase.phase1!=15){
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  checkRegister(mem[c->pc].phase.phase2);
  int destination=c->reg[mem[c->pc].phase.phase2];
  nop(1, c);
  shift.typeCh.b1=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b2=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b3=mem[c->pc].cbyte; nop(1, c);
  shift.typeCh.b4=mem[c->pc].cbyte; nop(1, c);
  destination=destination+shift.typeInt;
  if(destination<0 || destination>=c->size){
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
    exit(0);
  }
  if(operator!=0){
    if(destination+4<0 || destination+4>=c->size){
      fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
      exit(0);
    }
    shift.typeCh.b4=mem[destination+3].cbyte;
    shift.typeCh.b3=mem[destination+2].cbyte;
    shift.typeCh.b2=mem[destination+1].cbyte;
    shift.typeCh.b1=mem[destination].cbyte;

    printf("%d", shift.typeInt);
  }else {
    printf("%c", mem[destination].cbyte);
  }
}
void addl(cpu* c){
  int rA=c->reg[mem[c->pc].phase.phase2];
  int rB=c->reg[mem[c->pc].phase.phase1];
  if(rA>0 && rB>0 && rA+rB<0){
    c->OF=1;
    fprintf(stderr, "%s\n", "ERROR: Overflow");
    exit(0);
  }else if(rA<0 && rB<0 && rA+rB>0){
    c->OF=1;
    fprintf(stderr, "%s\n", "ERROR: Overflow");
    exit(0);
  }else{
    c->OF=0;
  }
  c->reg[mem[c->pc].phase.phase1]=rB+rA;
  if(c->reg[mem[c->pc].phase.phase1]==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(c->reg[mem[c->pc].phase.phase1]<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
void subl(cpu* c){
  int rA=c->reg[mem[c->pc].phase.phase2];
  int rB=c->reg[mem[c->pc].phase.phase1];
  if(rA<0 && rB>0 && rA-rB>0){
    c->OF=1;
    fprintf(stderr, "%s\n", "ERROR: Overflow");
    exit(0);
  }else if(rA>0 && rB<0 && rA-rB<0){
    c->OF=1;
    fprintf(stderr, "%s\n", "ERROR: Overflow");
    exit(0);
  }else{
      c->OF=0;
  }
  c->reg[mem[c->pc].phase.phase1]=rB-rA;
  if(c->reg[mem[c->pc].phase.phase1]==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(c->reg[mem[c->pc].phase.phase1]<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
void andl(cpu* c){
  int newVal=c->reg[mem[c->pc].phase.phase2] & c->reg[mem[c->pc].phase.phase1];
  c->OF=0;
  if(newVal==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(newVal<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
void xorl(cpu* c){
  int newVal=c->reg[mem[c->pc].phase.phase2] ^ c->reg[mem[c->pc].phase.phase1];
  c->OF=0;
  if(newVal==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(newVal<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
void mull(cpu* c){
  int rA=c->reg[mem[c->pc].phase.phase2];
  int rB=c->reg[mem[c->pc].phase.phase1];
  if(rA>0 && rB>0 && rA * rB<0){
    c->OF=1;
  }else if(rA<0 && rB<0 && rA * rB<0){
    c->OF=1; fprintf(stderr, "%s\n", "ERROR: Overflow"); exit(0);
  }else if(rA>0 && rB<0 && rA * rB>0){
    c->OF=1; fprintf(stderr, "%s\n", "ERROR: Overflow"); exit(0);
  }else if(rA<0 && rB>0 && rA * rB>0){
    c->OF=1; fprintf(stderr, "%s\n", "ERROR: Overflow");exit(0);
  }else{
    c->OF=0;
  }
  c->reg[mem[c->pc].phase.phase1]=rB * rA;
  if(c->reg[mem[c->pc].phase.phase1]==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(c->reg[mem[c->pc].phase.phase1]<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
void cmpl(cpu* c){
  int rA=c->reg[mem[c->pc].phase.phase2];
  int rB=c->reg[mem[c->pc].phase.phase1];
  c->OF=0;
  int res= rB-rA;
  if(res==0){
    c->ZF=1;
  }else{
    c->ZF=0;
  }
  if(res<0){
    c->SF=1;
  }else{
    c->SF=0;
  }
}
long checkAddress(long address, cpu* cpu){ //error check to ensure address is valid
    if(address>=cpu->size || address<0){ //if address is greater than size of cpu or is negative
        fprintf(stderr, "%s\n", "ERROR: Invalid address. Please try again.");
        return 0;
    }
    return address; //else address is valid
}
long checkHex(char ch){ //error check to ensure valid Hex value
    if(ch>='0' && ch<='9'){ //for when between 0-9 (numerals)
        return ch-'0';
    }else if(ch>='A' && ch<='F'){ //for when between A-F (UPPER CASE)
        return 10+ch-'A';
    }else if(ch>='a' && ch<='f'){ //for when between a-f (lower case)
        return 10+ch-'a';
    }else{ //when not a hex character, failure!
        fprintf(stderr, "%s\n", "ERROR: Invalid entry. Hex characters are 0-9, A-F (case insensitive).");
        return 0;
    }
}
long BinToDec(int* binary){ //convert bin to dec (taken from pa1)
  long decimal=0;
  int i;
  for(i=64; i>=0; i--){
    if(binary[i]==1){
      decimal+=binaryHelper(2,63-i);
    }
  }
  return decimal;
}
long binaryHelper(int power, int size){ //helper method for bin to dec conversion
  long i, result=1;
  for(i=1; i<=size; i++){
    result=result*power;
  }
  return result;
}
long HexToDec(char* hex){ //convert hex to dec (taken from pa1)
  int* binary=HexToBin(hex);
  long decimal=BinToDec(binary);
 	return decimal;
}
int* HexToBin(char* hex){ // convert hex to bin (taken from pa1)
  int* binary=malloc(64 * sizeof(int));
  int dig3, dig2, dig1, dig0, i, bsize=63;
  for(i=0; i<64; i++){
    binary[i]=0;
  }
  for(i=(strlen(hex))-1 ; i>=0; i--){
    switch(hex[i]){
      case '0': dig3=0; dig2=0; dig1=0; dig0=0; break;
      case '1': dig3=0; dig2=0; dig1=0; dig0=1; break;
      case '2': dig3=0; dig2=0; dig1=1; dig0=0; break;
      case '3': dig3=0; dig2=0; dig1=1; dig0=1; break;
      case '4': dig3=0; dig2=1; dig1=0; dig0=0; break;
      case '5': dig3=0; dig2=1; dig1=0; dig0=1; break;
      case '6': dig3=0; dig2=1; dig1=1; dig0=0; break;
      case '7': dig3=0; dig2=1; dig1=1; dig0=1; break;
      case '8': dig3=1; dig2=0; dig1=0; dig0=0; break;
      case '9': dig3=1; dig2=0; dig1=0; dig0=1; break;
      case 'A': case 'a': dig3=1; dig2=0; dig1=1; dig0=0; break;
      case 'B': case 'b': dig3=1; dig2=0; dig1=1; dig0=1; break;
      case 'C': case 'c': dig3=1; dig2=1; dig1=0; dig0=0; break;
      case 'D': case 'd': dig3=1; dig2=1; dig1=0; dig0=1; break;
      case 'E': case 'e': dig3=1; dig2=1; dig1=1; dig0=0; break;
      case 'F': case 'f': dig3=1; dig2=1; dig1=1; dig0=1; break;
      default: fprintf(stderr, "%s\n", "ERROR: Invalid entry. Hex characters are 0-9, A-F (case insensitive)."); return 0;
    }
    binary[bsize--]=dig0;
    binary[bsize--]=dig1;
    binary[bsize--]=dig2;
    binary[bsize--]=dig3;
  }
  return binary;
}
void rrmovl(cpu* c){ //RRMOVL INSTRUCTION
  if(mem[c->pc].phase.phase1!=0){ //error statement when mem is not 0
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }nop(1,c);
  checkRegister(mem[c->pc].phase.phase2); checkRegister(mem[c->pc].phase.phase1); //ensure regs at phase 1 and phase 2 are less or equal to 7.
  c->reg[mem[c->pc].phase.phase1]=c->reg[mem[c->pc].phase.phase2]; //reg at phase 2 is equal to reg at phase 1
  nop(1,c);
}
void irmovl(cpu* c){ //IRMOVL INSTRUCTION
	int unsigned reg=0;
	convchar ch;
  if(mem[c->pc].phase.phase1!=0){ //mem at phase 2 is not 0
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  nop(1,c); //nop by 1
  if(mem[c->pc].phase.phase2!=15){ //when phase 1 memory is not 15  - stop
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  reg=checkRegister(mem[c->pc].phase.phase1);
  nop(1,c); ch.typeCh.b1=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b2=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b3=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b4=mem[c->pc].cbyte; //nop by 1; set character to memory
  c->reg[reg]=ch.typeInt; nop(1,c);
}
void rmmovl(cpu* c){ //RMMOVL INSTRUCTION
  if(mem[c->pc].phase.phase1!=0){ //when phase 2 mem is not 0
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }nop(1,c); //else when it is 0
  int unsigned rA=checkRegister(mem[c->pc].phase.phase2); //check reg to make sure less than 8
  int unsigned rB=checkRegister(mem[c->pc].phase.phase1); //check reg to make sure less than 8
  convchar ch, output;
  nop(1,c); ch.typeCh.b1=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b2=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b3=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b4=mem[c->pc].cbyte; //nop by 1; set character to memory
  ch.typeInt=ch.typeInt+c->reg[rB];
  if(ch.typeInt<0 || ch.typeInt+3>=c->size){
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address has been encountered. Y86 Program execution stopped.");
    exit(0);
  }
  output.typeInt=c->reg[rA];
  mem[ch.typeInt].cbyte=output.typeCh.b1; //set mem equal to output
  mem[ch.typeInt+1].cbyte=output.typeCh.b2;  //set mem equal to output
  mem[ch.typeInt+2].cbyte=output.typeCh.b3;  //set mem equal to output
  mem[ch.typeInt+3].cbyte=output.typeCh.b4;  //set mem equal to output
  nop(1,c);
}
void mrmovl(cpu* c){ //MRMOVL INSTRUCTION
  if(mem[c->pc].phase.phase1!=0){ //error check - when not 0
    //loads a word from memory
    //one of two instructions that access memory
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }nop(1,c);
  int unsigned rA=checkRegister(mem[c->pc].phase.phase2); //ensure reg is <=7
  int unsigned rB=checkRegister(mem[c->pc].phase.phase1); //ensure reg is <=7
  convchar ch, output;
  nop(1,c); ch.typeCh.b1=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b2=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b3=mem[c->pc].cbyte; //nop by 1; set character to memory
  nop(1,c); ch.typeCh.b4=mem[c->pc].cbyte; //nop by 1; set character to memory
  ch.typeInt+=c->reg[rB];
  if(ch.typeInt< 0||ch.typeInt+3>=c->size){
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address. Y86 Program execution stopped.");
    exit(0);
  }
  output.typeCh.b1=mem[ch.typeInt].cbyte; //set output equal to memory
  output.typeCh.b2=mem[ch.typeInt+1].cbyte; //set output equal to memory
  output.typeCh.b3=mem[ch.typeInt+2].cbyte; //set output equal to memory
  output.typeCh.b4=mem[ch.typeInt+3].cbyte; //set output equal to memory
  c->reg[rA]=output.typeInt; nop(1,c);
}
void movsbl(cpu* c){ //MOVSBL INSTRUCTION
  //extends a byte and adds leading ones (sign extended) to the register
  convchar shift;
  if(mem[c->pc].phase.phase1!=0){ //error check - when not equal to 0 = ERROR
    fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
    exit(0);
  }
  nop(1, c); //nop by 1
  checkRegister(mem[c->pc].phase.phase2); checkRegister(mem[c->pc].phase.phase1); //ensure regs at phase 1 and phase 2 are less or equal to 7.
  int storageReg=mem[c->pc].phase.phase2; //store register stores phase 1
  int ch=c->reg[mem[c->pc].phase.phase1];
  nop(1, c);
  shift.typeCh.b1=mem[c->pc].cbyte; nop(1, c); //shift equal mem, nop by 1
  shift.typeCh.b2=mem[c->pc].cbyte; nop(1, c); //shift equal mem, nop by 1
  shift.typeCh.b3=mem[c->pc].cbyte; nop(1, c); //shift equal mem, nop by 1
  shift.typeCh.b4=mem[c->pc].cbyte; nop(1, c); //shift equal mem, nop by 1
  ch+=shift.typeInt;
  if(ch<0 || ch>=c->size){ //when negative or greater than size; ERROR
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
    exit(0);
  }
  if(mem[ch].phase.phase2>9){ //when greater than 9
    shift.typeInt=-1;
    shift.typeCh.b1=mem[ch].cbyte;
  }else{
    shift.typeInt=0;
    shift.typeCh.b1=mem[ch].cbyte;
  }
  c->reg[storageReg]=shift.typeInt;
}
void opl(cpu* c){ //OPl INSTRUCTION
  //when processor executes an OPl instruction, the condition codes generated by the ALU will be saved in the condition code registers
  int operator=mem[c->pc].phase.phase1; //set operator to phase 2 memory
  nop(1, c);
  switch(operator){ //switch statement for the 6 opcodes
    case 0: addl(c); break; //add
    case 1: subl(c); break; //sub
    case 2: andl(c); break; //and
    case 3: xorl(c);  break; //xor
    case 4: mull(c); break; //mul
    case 5: cmpl(c); break; //compare
    default: fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped."); exit(0);
  }
  nop(1, c);
}
void jmp(cpu* c){ //JMP INSTRUCTION
  convchar ch;
  ch.typeCh.b1=mem[c->pc].cbyte; nop(1, c); //set character to memory, nop by 1
  ch.typeCh.b2=mem[c->pc].cbyte; nop(1, c); //set character to memory, nop by 1
  ch.typeCh.b3=mem[c->pc].cbyte; nop(1, c); //set character to memory, nop by 1
  ch.typeCh.b4=mem[c->pc].cbyte;  //set character to memory
  if(ch.typeInt<0||ch.typeInt>=c->size){ //error check for when negative or greater than size
    fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
    exit(0);
  }
  c->pc=ch.typeInt;
}
void jcon(cpu* c){ //JCON INSTRUCTION
  int operator=mem[c->pc].phase.phase1; //phase 2 mem --> operator
  nop(1, c);
  switch(operator){ //switch statement (from class notes)
    case 0: jmp(c); break;
    case 1: if((c->SF^c->OF)==1 || c->ZF==1){jmp(c);}
                 else{nop(4, c);} break; //sign*overflow = 1 OR zero=1
    case 2: if((c->SF ^ c->OF)==1){jmp(c);}
                 else{nop(4, c);} break; //sign*overflow = 1
    case 3: if(c->ZF==1){jmp(c);}
                 else{nop(4, c);} break; //zero = 1
    case 4: if(c->ZF==0){jmp(c);}
                 else{nop(4, c);} break; //zero=0
    case 5: if((c->SF ^ c->OF)==0){jmp(c);}
                 else{nop(4, c);} break; //sign*overflow = 0
    case 6: if((c->SF ^ c->OF)==0 && c->ZF==0){ jmp(c);}
                 else{nop(4, c);} break; //sign*overflow = 0 AND zero=0
    default:  fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped."); exit(0); //default error
  }
}
void call(cpu* c){ //CALL INSTRUCTION
  convchar ch, result;
  if(mem[c->pc].phase.phase1!=0){ //error check for when mem is not 0
      fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
      exit(0);
   }
   nop(1, c);
   ch.typeCh.b1=mem[c->pc].cbyte; nop(1, c); //char equals mem, nop by 1
   ch.typeCh.b2=mem[c->pc].cbyte; nop(1, c); //char equals mem, nop by 1
   ch.typeCh.b3=mem[c->pc].cbyte; nop(1, c); //char equals mem, nop by 1
   ch.typeCh.b4=mem[c->pc].cbyte; nop(1, c); //char equals mem, nop by 1
   result.typeInt=c->pc; //result is when cpu points to pc
   if(c->reg[4]-4<=0) { //when less than or equal to 0
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }
   mem[c->reg[4] - 4].cbyte=result.typeCh.b1; //mem equals result
   mem[c->reg[4] - 3].cbyte=result.typeCh.b2; //mem equals result
   mem[c->reg[4] - 2].cbyte=result.typeCh.b3; //mem equals result
   mem[c->reg[4] - 1].cbyte=result.typeCh.b4; //mem equals result
   c->reg[4] -= 4; //reduce by 4
   if(ch.typeInt<0 || ch.typeInt>=c->size){ //error check for NEGATIVE INPUTS (for prog 2!!!)
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Negative number enterred. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }
   c->pc=ch.typeInt;
 }
void ret(cpu* c){ //RET INSTRUCTION
   convchar result; //return from procedure
   if(mem[c->pc].phase.phase1!=0){ //error check when not 0
     fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
     exit(0);
   }
   if(c->reg[4]+3>=c->size) { //error check for invalid inputs
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }
   result.typeCh.b4=mem[c->reg[4]+3].cbyte; //result is equal to mem by 3
   result.typeCh.b3=mem[c->reg[4]+2].cbyte; //result is equal to mem by 2
   result.typeCh.b2=mem[c->reg[4]+1].cbyte; //result is equal to mem by 1
   result.typeCh.b1=mem[c->reg[4]].cbyte; //result is equal to mem by 0
   if(result.typeInt<0||result.typeInt>=c->size){ //error check for invalids
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }
   c->reg[4]+=4;
   c->pc=result.typeInt;
 }
void pushl(cpu* c){ //PUSH INSTRUCTION
   convchar result;
   if(mem[c->pc].phase.phase1!=0){ //error check for invalid input / INS
     fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
     exit(0);
   }
   nop(1, c);
   checkRegister(mem[c->pc].phase.phase2);
   if(mem[c->pc].phase.phase1!=15){ //error check for invalid input / INS
     fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
     exit(0);
   }
   if(c->reg[4] - 4<=0) { //error check for invalid input / ADR
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }
   result.typeInt=c->reg[mem[c->pc].phase.phase2]; //pushing on to stack
   mem[c->reg[4] - 4].cbyte=result.typeCh.b1; //pushing on to stack
   mem[c->reg[4] - 3].cbyte=result.typeCh.b2; //pushing on to stack
   mem[c->reg[4] - 2].cbyte=result.typeCh.b3; //pushing on to stack
   mem[c->reg[4] - 1].cbyte=result.typeCh.b4; //pushing on to stack
   c->reg[4] -= 4;
   nop(1, c);
 }
void popl(cpu* c){ //POP INSTRUCTION
   if(mem[c->pc].phase.phase1!=0){ //error check for INS
     fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
     exit(0);
   }
   nop(1, c);
   checkRegister(mem[c->pc].phase.phase2); //ensure phase 1 memory register is <=7
   if(mem[c->pc].phase.phase1!=15){ //error check for INS
     fprintf(stderr, "%s\n", "ERROR: INS: Invalid instruction. Y86 Program execution stopped.");
     exit(0);
   }if(c->reg[4]+3>=c->size) { //error check for invalid input
     fprintf(stderr, "%s\n", "ERROR: Invalid Input. Please only enter positive numbers between 0-12. // ADR: Invalid address encountered. Y86 Program execution stopped.");
     exit(0);
   }convchar forReg;
   forReg.typeCh.b1=mem[c->reg[4]].cbyte; //pop off stack
   forReg.typeCh.b2=mem[c->reg[4]+1].cbyte;  //pop off stack
   forReg.typeCh.b3=mem[c->reg[4]+2].cbyte;  //pop off stack
   forReg.typeCh.b4=mem[c->reg[4]+3].cbyte;  //pop off stack
   c->reg[4] += 4;
   c->reg[mem[c->pc].phase.phase2]=forReg.typeInt;  //pop off stack
   nop(1, c);
 }
int decode(char*  file){ //DECODE
   //part of fetch-decode-execute
   cpu*c;
   char* str=NULL; //had to set null bc of warnings
   int value;
   long size, addr;
   char* arg, *ptr, *hadd, *tmp;
   str=strdup(file);
   tmp=strtok(str, "\t\n");
   convchar ch;
   while(tmp!=NULL){ //while temp is NOT null
     if(strcmp(tmp, ".size")==0){ //if .size is 0
       tmp=strtok(NULL, "\t\n");
       size=(int) strtol(tmp, NULL, 16);
       mem=malloc(size*sizeof(byte));
     }else{ //else do this
       tmp=strtok(NULL, "\t\n");
     }
   }
   free(str); //free malloc
   c=malloc(sizeof(*c)); //set c to malloc of size c
   c->pc=-1; //decrement pc by 1
   c->size=size; //set size equal to size when pointed to by cpu
   int i=0; while(i<8){ //from 0-8 (9 times)
     c->reg[i]=0; //stores 9 times
     i++;
   }
   str=strdup(file); //strdup file
   tmp=strtok(str, "\t\n");
   while(tmp){
     if(strcmp(tmp, ".size")==0){ //when .size is zero
       tmp=strtok(NULL, "\t\n");
     }else if(strcmp(tmp, ".string")==0){ //when .string is zero
       tmp=strtok(NULL, "\t\n");
       hadd=tmp;
       addr=HexToDec(hadd); //convert hex to decimal to find address
       arg=strtok(NULL, "\t\n");
       ptr=arg+1; //move pointer one past arg
       while(*ptr!='\0'){ //ptr is null
         addr=checkAddress(addr, c); //error check
         mem[addr].cbyte=*ptr; //if no error
         ptr++; addr++; //increment!!!!!
       }
     }else if(strcmp(tmp, ".long")==0){ //if long is zero
       tmp=strtok(NULL, "\t\n"); //do this
       hadd=tmp;
       addr=HexToDec(hadd);
       arg=strtok(NULL, "\t\n");
       ch.typeInt=(int) strtol(arg, NULL, 16);
       mem[checkAddress(addr, c)].cbyte=ch.typeCh.b1;  //set ch in memory
       mem[checkAddress(addr, c)+1].cbyte=ch.typeCh.b2;  //set ch in memory
       mem[checkAddress(addr, c)+2].cbyte=ch.typeCh.b3;  //set ch in memory
       mem[checkAddress(addr, c)+3].cbyte=ch.typeCh.b4;  //set ch in memory
     }else if(strcmp(tmp, ".byte")==0){ //when .byte is zero
       tmp=strtok(NULL, "\t\n");
       hadd=tmp;
       addr=HexToDec(hadd); //set hex's address to address
       arg=strtok(NULL, "\t\n");
       value=(int) strtol(arg, NULL, 16);
       mem[checkAddress(addr, c)].cbyte=(char) value;
     }else if(strcmp(tmp, ".text")==0){ //when .text is zero
       tmp=strtok(NULL, "\t\n");
       hadd=tmp;
       addr=HexToDec(hadd);
       c->pc=(int)addr;
       arg=strtok(NULL, "\t\n");
       ptr=arg;
       while(*ptr!='\0' && isspace(*ptr)==0){
         addr=checkAddress(addr, c);
         mem[addr].phase.phase2=checkHex(*ptr);
         ptr++;
         if(*ptr!= '\0' && isspace(*ptr)==0){
           mem[addr].phase.phase1=checkHex(*ptr);
           ptr++;
         }
         addr++;
       }
     }
     tmp=strtok(NULL, "\t\n");
   }
   int instr=0;
   while(instr!=-1){
     instr=mem[c->pc].phase.phase2;
     switch(instr){ //instruction codes!!
       case 0:	nop(1, c); break; //nop
       case 1:	halt(-1); break; //halt to stop
       case 2:	rrmovl(c); break; //rrmovl
       case 3:	irmovl(c);	break; //irmovl
       case 4:	rmmovl(c); break; //rmmovl
       case 5:	mrmovl(c);	break; //mrmovl
       case 6:	opl(c);	break; //opcodes switch statement called
       case 7:	jcon(c); break; //jcon
       case 8:	call(c);  break; //call
       case 9:	ret(c); break; //return
       case 10: pushl(c); break; //push
       case 11: popl(c); break; //pop
       case 12: readX(c); break; //read
       case 13: writeX(c); break; //write
       case 14: movsbl(c); break; //movsbl
       default: fprintf(stderr, "%s\n", "ERROR: Invalid entry. Please try again."); exit(0); //default error
     }
   }
    return 0; //success
}
int main(int argc, char *argv[]){ //MAIN METHOD
   if(argc!=2){ //when less than or more than 2 args
     fprintf(stderr, "%s\n", "ERROR: Number of inputs entered was either greater or less than 2. Valid input is $ ./y86emul <filename>");
     return 1; //failure
   }else{ //if number of arguments is exactly 2
    if(strcmp(argv[1], "-h")==0){ //usage message
       fprintf(stderr, "%s\n", "Usage: ./y86emul <y86 file name>");
       return 1; //failure
     }
    char* input=strstr(argv[1],".y86");
    if(input==NULL || strlen(input)!=4 || strlen(argv[1])<5){
      fprintf(stderr, "%s\n", "ERROR: Invalid file. Must be a .y86 file. Please try again.");
      return 1; //failure
    }
     decode(readB(argv[1])); //check if arg is even in the folder/valid
     return 0; //success
   }
 }

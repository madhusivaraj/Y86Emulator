#ifndef _Y86EMUL_H_
#define _Y86EMUL_H_

typedef struct cpu{ //struct called cpu
  int reg[8], pc, size; //registers of size 8; program counter; size
  int unsigned OF, ZF, SF; //flags for overflow, zero and sign
} cpu;
typedef union{ //union for phases
  struct{
    int unsigned phase1: 4; //phase1
    int unsigned phase2: 4; //phase2
  } phase; //the phases
  char cbyte;
} byte;
typedef union{ //union for bytes as chars
  struct{
    char b1, b2, b3, b4;
  }typeCh;
  int typeInt;
}convchar;
byte* mem;

void nop(int, cpu*);
void halt(int);
char* readB(char*);
int checkRegister(int);
void readX(cpu*);
void writeX(cpu*);
void irmovl(cpu*);
void rrmovl(cpu*);
void addl(cpu*);
void subl(cpu*);
void andl(cpu*);
void xorl(cpu*);
void mull(cpu*);
void cmpl(cpu*);
long checkAddress(long, cpu*);
long checkHex(char);
long BinToDec(int*);
long binaryHelper(int, int);
long HexToDec(char*);
int* HexToBin(char*);
void mrmovl(cpu*);
void rmmovl(cpu*);
void movsbl(cpu*);
void opl(cpu*);
void jmp(cpu*);
void jcon(cpu*);
void call(cpu*);
void ret(cpu*);
void pushl(cpu*);
void popl(cpu*);
int decode(char*);
#endif

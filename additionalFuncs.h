/***********************************
* By Matan Liber and Shalhav Harkavi
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32
#define MAX_LINE_LENGTH 82
#define true 1
#define false 0

typedef enum LabelType{none, data, string}type;

typedef enum AdditionalLabelType{none, entry, external}addType;

typedef struct Label
{
	char name[MAX_NAME_LENGTH];
	int adress;
	type id;
	addType addId;
	int *value;
	char *string;
	struct Label *next;
}label;

int isEqual(char str1[], char str2[]);

int isLegalName(char str[]);

int isLabel(char str[]);

char *getLabelName(char str[]);

type getType(char str[]);

addType getAddType(char str[]);

int *getValue(char str[], type id);

char *getString(char str[], type id);

typedef enum {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop} opCode;

typedef enum {r1, r2, r3, r4, r5, r6, r7, r8} registers;

typedef enum {immidiate, direct, jmpWparam, directReg} addresing;

typedef struct {unsigned short int ARE:2;
                unsigned short int data:12;
                } AREdataWord;

typedef struct {unsigned short int data:14;} dataWord;

typedef struct {unsigned short int ARE:2;
                unsigned short int destination:6;
                unsigned short int source:6;
                } registerAdressWord;

typedef struct {unsigned short int ARE:2;
                unsigned short int destAddresing:2;
                unsigned short int sourceAddresing:2;
                unsigned short int op:4;
                unsigned short int param1:2;
                unsigned short int param2:2;
                } instructionWord;

typedef struct {unsigned short int ARE:2;
                unsigned short int address:12;
                } AREaddressWord;

void instruction(char *str); /*public function for instruction words creation*/

typedef union {AREaddressWord     AREaddress;
               instructionWord    instruction;
               registerAdressWord registerAddress;
               dataWord           data;
               AREdataWord        AREdata;
               } word;

void makeDataWords(int data); /*public function for none ARE data words creation*/

typedef struct BinWordList {word Word;
                            struct BinWordList *next;
                            struct BinWordList *prev;
                            } wordList;

wordList *instructions; /*pointer to the first instruction*/
wordList *data;         /*pointer to the first piece of data*/

int isDataLabel(char str[]);

int isInstructionLabel(char str[]);

void instruction(char str[]);

void error(char code);

void data(label* labelData)

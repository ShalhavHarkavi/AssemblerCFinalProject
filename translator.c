/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"

typedef enum {notAnOp = -1, mov, cmp, add, sub, not, clr, lea,
              inc, dec, jmp, bne, red, prn, jsr, rts, stop} opCode;

typedef enum {r1, r2, r3, r4, r5, r6, r7, r8} registers;

typedef enum {immidiate, direct, jmpWparam, directReg} addresing;

typedef enum {absolute, external, relocatable} ARE;

typedef struct {unsigned short int are:2;
                unsigned short int Data:12;
                } AREdataWord;

typedef struct {unsigned short int Data:14;} dataWord;

typedef struct {unsigned short int are:2;
                unsigned short int destination:6;
                unsigned short int source:6;
                } registerAdressWord;

typedef struct {unsigned short int are:2;
                unsigned short int destAddresing:2;
                unsigned short int sourceAddresing:2;
                unsigned short int op:4;
                unsigned short int param1:2;
                unsigned short int param2:2;
                } instructionWord;

typedef struct {unsigned short int are:2;
                unsigned short int address:12;
                } AREaddressWord;


typedef union {AREaddressWord     AREaddress;
               instructionWord    instruction;
               registerAdressWord registerAddress;
               dataWord           dword;
               AREdataWord        AREdata;
               } word;

typedef struct WordList {word Word;
                         struct WordList *next;
                         } wordList;

typedef union {unsigned short int twelveBit:12;
               unsigned short int fourteenBit:14;
              } bitData;

/*These are actually one linked list with the last instruction's next pointing *
* to the first data and the first. The two pointers are just used to minimize  *
* the number of required offset calculation.                                   */
static unsigned int IC; /* Instruction Counter */
static unsigned int DC; /* Data Counter */
static wordList *instructionHead; /*pointer to the first instruction*/
static wordList *currentInst;     /*pointer to the last inserted instruction*/              
static wordList *dataHead;        /*pointer to the first piece of data*/
static wordList *currentData;     /*pointer to the last inserted data word*/         
static void insertDataWord(word wrd); /* insert a data word to the end of the data list*/
static void insertinstructionWord(word wrd); /* insert a data word to the end of the data list*/

static opCode whatOpCode(char *str); /*identify op and remove it*/
/*binary data words with or without ARE prefix*/
static word makeDataWords(ARE *are, int); 
/*binary address word either register or label*/
/*instruction (first) word*/
static 
static word makeAdressWord(ARE are, registers *regDest, registers *regSource, unsigned char *address); 
static bitData TwosComplement(unsigned int data, char bits);
static char *skipBlanks(char *str); /*return pointer to first non-blank beginnig of str*/

void initializeWordList(void){
  wordList *instList = (wordList*)malloc(sizeof(wordList));
  wordList *dataList = (wordList*)malloc(sizeof(wordList));
  instList -> next = dataList;
  dataList -> next = NULL;
  IC   = 0;
  DC   = 0;
  instructionHead  = instList;
  dataHead         = dataList;
  currentInst      = instructionHead;
  currentData      = dataHead;
}

void clearWordList(wordList *head){
  while (head -> next != NULL){
    wordList *temp = head -> next;
    free(head);
    head = temp;
  }
  free(head);
}

void data(label* labelData)
{
  int i, length;
  if (labelData.id == data) {
    length = sizeof(labelData -> value) / sizeof(int); /*Size of value array*/
    for (i = 0; i < length; i++) {
      insertDataWord(makeDataWords(NULL, (labelData -> value)[i]));
    }
  }
  else if (labelData.id == string) {
    length == strlen(labelData -> string) + 1; /*Length of word, +1 for '\0'*/
    for (i = 0; i < length; i++) {
      insertDataWord(makeDataWords(NULL, (int) (labelData -> string)[i]));
    }
  }
}

void insertDataWord(word wrd){
  wordList *nextWordInList = (wordList*)malloc(sizeof(wordList));
  nextWordInList -> Word = wrd;
  nextWordInList -> next = NULL;
  currentData    -> next = nextWordInList;
  currentData    = nextWordInList;
  DC++;
}

void instruction(char *str, label* labelInstruction) {
  const char *working = skipBlanks(str);
  instructionWord *instWord = (instructionWord*)malloc(sizeof(instructionWord));
  instWord -> are = 0;
  if (opCode op=whatOpCode(working) < 0)
    error(1);
  else {
    instWord -> op = op;
    if (op>=rts) {
      instWord -> destAddresing = 0;
      instWord -> param1 = 0;
      instWord -> param2 = 0;
    }
    if (op == not|| op == clr || op >= inc) {
      instWord -> sourceAddresing = 0;
      working = skipBlanks(working+3);
      if (working == '#'){
        int i
        instWord -> destAddresing = 0;
        instWord -> param1 = 0;
        instWord -> param2 = 0;
        ARE are = 0;
        for (i=1;isdigit(*(working+i)) || *(working+i)=='-'; i++)
          ;
        char *num = malloc(i);
        strncpy(num, (const char*)working+1, i);
        wordList *newInst = (wordList*)malloc(sizeof(wordList));
        newInst -> word = instWord;
        wordList *dWord   = (wordList*)malloc(sizeof(wordList));
        dword -> word = makeDataWords(&are, atoi(num));
        dword -> next = currentInst -> next;
        currentInst -> next = newInst;
        newInst -> next = dWord;
        instructionHead -> IC += 2;
        if (skipBlanks(working + i) != '\0')
          error(12);
        return;
      }
      else if (*working == 'r' && *(working+1) >= '0' && *(working+1) <= '7') {
        instWord -> destAddresing = 3;
        instWord -> param1 = 0;
        instWord -> param2 = 0;
        ARE are = 0;
      }
    }

  } 
}

static word makeAdressWord(ARE are, registers *regDest, registers *regSource, unsigned char *address){
  word wrd;
  wrd.AREaddress.are;
  if (address == NULL){
      wrd.registerAddress.destination = *regDest;
    if (regSource != NULL)
      wrd.registerAddress.source = *regSource;
    else
      wrd.registerAddress.source = 0;
  }
  else
    wrd.AREaddress.address = *address;
  return wrd;
}

bitData TwosComplement (unsigned int data, char bits) {
  bitData myNum;
  switch(bits){
    case 12:
      myNum.twelveBit = data;
      myNum.twelveBit = ~myNum.twelveBit + 1;
      break;
    case 14:
      myNum.fourteenBit = data;
      myNum.fourteenBit = ~myNum.fourteenBit + 1;
      break;
  return myNum;
  }
}

char *Word2CommaSlash(word wrd){
  char *comSlWord = (char*)malloc(15*sizeof(char));
  for (int i=13;i>=14;i--){
    if (wrd.dword.Data % 2)
      *(comSlWord + i) = '1';
    else
      *(comSlWord + i) = '0';
    wrd.dword.Data /= 2;
  }
  *(comSlWord + 14) = '\0';
  return comSlWord;
}

word makeDataWords(ARE *Are, signed int num){
  word newWord;
  if (Are==NULL) {
    if (num < 0)
      newWord.dword.Data = (unsigned short int)(TwosComplement((unsigned int) -num,14).fourteenBit);  
    else
      newWord.dword.Data = num;
  }  
  else {  
    newWord.AREdata.are = *Are;
    if (num < 0)
      newWord.AREdata.Data = (unsigned short int)(TwosComplement((unsigned int) -num,12).twelveBit);  
    else
      newWord.AREdata.Data = num;
  }
  return newWord;
}

opCode whatOpCode(char *str){
  char op[4];
  for (int i=0;i<3;i++)
    *(op+i) = *(str+i);
  op[3]='\0';
  if (!strcmp(op, "mov"))
    return mov;
  else if (!strcmp(op, "cmp"))
    return cmp;
  else if (!strcmp(op, "add"))
    return add;
  else if (!strcmp(op, "sub"))
    return sub;
  else if (!strcmp(op, "not"))
    return not;
  else if (!strcmp(op, "clr"))
    return clr;
  else if (!strcmp(op, "lea"))
    return lea;
  else if (!strcmp(op, "inc"))
    return inc;
  else if (!strcmp(op, "dec"))
    return dec;
  else if (!strcmp(op, "jmp"))
    return jmp;
  else if (!strcmp(op, "bne"))
    return bne;
  else if (!strcmp(op, "red"))
    return red;
  else if (!strcmp(op, "prn"))
    return prn;
  else if (!strcmp(op, "jsr"))
    return jsr;
  else if (!strcmp(op, "rts"))
    return rts;
  else if (!strcmp(op, "sto"))
    if (*(str+3) == 'p')
      return stop;
  else
    return notAnOp;
}

char *skipBlanks(char *str){
  char *output = str;
  for(;isblank((int) *output) && *output != '\0';output++)
    ;
  return output;
}
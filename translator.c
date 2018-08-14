/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"

typedef enum {notAnOp = -1, mov, cmp, add, sub, not, clr, lea,
              inc, dec, jmp, bne, red, prn, jsr, rts, stop} opCode;

typedef enum {r1, r2, r3, r4, r5, r6, r7, r8} registers;

typedef enum {immidiate, direct, jmpWparam, directReg} Addressing;

typedef enum {Absolute, External, Relocatable} ARE;

typedef struct {unsigned int are:2;
                unsigned int DATA:12;
                } AREdataWord;

typedef struct {unsigned int DATA:14;} dataWord;

typedef struct {unsigned int are:2;
                unsigned int destination:6;
                unsigned int source:6;
                } registerAdressWord;

typedef struct {unsigned int are:2;
                unsigned int destAddressing:2;
                unsigned int sourceAddressing:2;
                unsigned int op:4;
                unsigned int param1:2;
                unsigned int param2:2;
                } instructionWord;

typedef struct {unsigned int are:2;
                unsigned int address:12;
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

typedef union {unsigned int twelveBit:12;
               unsigned int fourteenBit:14;
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
/*create a data word if immidiate data or return an illegal word with are=3*/
static word isImmidiate(char *str);
/*create a register word if str is a register or return an illegal word with are=3*/
static word isRegister(char *str);
/*create a label address word if str is a label name (can be External) or an illegal word with are=3 if not */
static word isDirect(char *str);
/*binary data words with or without ARE prefix*/
static word makeDataWords(ARE *are, int);
/*binary address word either register or label*/
static int isJmpWParams(char *str, word words[], opCode op); /*create words for jump with parameters */
/*instruction (first) word*/
static word makeInstruction(ARE are, Addressing dest, Addressing source, opCode opcode, Addressing param1, Addressing param2);
static word makeAdressWord(ARE are, registers *regDest, registers *regSource, unsigned int *address);
static bitData TwosComplement(unsigned int data, char bits);
static void updateLine(lines *currentLine);  /*update the memory position of line*/

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

void clearWordList(void){
  wordList *head = instructionHead;
  while (head -> next != NULL){
    wordList *temp = head -> next;
    free(head);
    head = temp;
  }
  free(head);
}

void Data(label *labelData, lines *currentLine)
{
  int i, length;
  labelData -> adress = DC;
  if (labelData -> id == data) {
    length = sizeof(labelData -> value) / sizeof(int); /*Size of value array*/
    for (i = 0; i < length; i++) {
      insertDataWord(makeDataWords(NULL, (labelData -> value)[i]));
      if (i == 0)
        updateLine(currentLine);
    }
  }
  else if (labelData -> id == string) {
    length = strlen(labelData -> string) + 1; /*Length of word, +1 for '\0'*/
    for (i = 0; i < length; i++) {
      insertDataWord(makeDataWords(NULL, (int) (labelData -> string)[i]));
      if (i == 0)
        updateLine(currentLine);
    }
  }
}

void insertDataWord(word wrd){
  wordList *nextWordInList = (wordList*)malloc(sizeof(wordList));
  nextWordInList -> Word = wrd;
  nextWordInList -> next = NULL;
  currentData    -> next = nextWordInList;
  currentData    =  nextWordInList;
  DC++;
}

void insertinstructionWord(word wrd){
  wordList *nextWordInList = (wordList*)malloc(sizeof(wordList));
  nextWordInList -> Word = wrd;
  nextWordInList -> next = NULL;
  currentInst    -> next = nextWordInList;
  currentInst    =  nextWordInList;
  IC++;
}

word isImmidiate(char *str) {
  word wrd;
  if (*str == '#'){
    int i;
    char *num;
    ARE Are = immidiate;
    for (i=1;isdigit((int)*(str+i)) || *(str+i)=='-'; i++)
      ;
    num = malloc(i);
    strncpy(num, (const char*)str+1, i);
    wrd = makeDataWords(&Are, atoi(num));
    str += i;
    free(num);
  }
  else
    wrd.AREdata.are = 3;
  return wrd;
}

word isRegister(char *str) {
  word wrd;
  if (*str == 'r' && *(str+1) >= '1' && *(str+1) <= '8') {
    registers reg = *(str+1) - '1';
    wrd = makeAdressWord(Absolute, NULL, &reg, NULL);
    str += 2;
  }
  else
    wrd.AREdata.are = 3;
  return wrd;
}

word isDirect(char *str) {
  word wrd;
  int i;
  char *labelName;
  for (i=0; isalpha((int)*(str+i)); i++)
    ;
  labelName = malloc(i+1);
  strncpy(labelName, str, i);
  if (isLegalName(labelName)){
    wrd = makeAdressWord(Relocatable, NULL, NULL, NULL);
    str += i;
  }
  else
    wrd.AREdata.are = 3;
  free(labelName);
  return wrd;
}

int isJmpWParams(char *str, word words[], opCode op) {
  int numOfWords = 0;
  char *working = str;
  Addressing param1;
  if ((words[1] = isDirect(working)).AREdata.are == 3)
    error(1);
  else
    numOfWords = 2;
  if (*working++ == '(') {
    if ((words[2] = isImmidiate(working)).AREdata.are != 3) {
      param1 = immidiate;
      numOfWords = 2;
    }
    else if ((words[2] = isDirect(working)).AREdata.are != 3) {
      param1 = direct;
      numOfWords = 2;
    }
    else if ((words[2] = isRegister(working)).AREdata.are != 3) {
      param1 = directReg;
      numOfWords = 2;
    }
    else
      return 0;
    if (*(working++) != ',')
      return 0;
    if ((words[3] = isDirect(working)).AREdata.are != 3) {
      words[0] = makeInstruction(Absolute, jmpWparam, immidiate, op, param1, direct);
      numOfWords++;
    }
    else if ((words[3] = isRegister(working)).AREdata.are != 3) {
      words[0] = makeInstruction(Absolute, jmpWparam, immidiate, op, param1, directReg);
      if (param1 == directReg) {
        words[2].registerAddress.destination = words[3].registerAddress.source;
      }
      else
        numOfWords++;
    }
    else if ((words[3] = isImmidiate(working)).AREdata.are != 3 && op == cmp) {
      words[0] = makeInstruction(Absolute, jmpWparam, immidiate, op, param1, immidiate);
      numOfWords++;
    }
    else
      return 0;
  }
  return numOfWords;
}

void instruction(char *str, label *labelInstruction, lines *currentLine) {
  char *working = skipBlanks(str);
  opCode op=whatOpCode(working);
  if (op < 0)
    error(0);
  else {
    word words[4];
    int i;
    unsigned char numOfWords = 0;
    Addressing sourceAddr;
    if (op>=rts) { /* first group no operands */
      words[0] = makeInstruction(Absolute, immidiate, immidiate, op, immidiate, immidiate);
      numOfWords = 1;
    }
    else if (op == mov || op == add || op == sub || op == cmp) { /* second group source addressing 0,1,3 */
      working = skipBlanks(working+3);
      if ((words[1] = isImmidiate(working)).AREdata.are != 3) {
        sourceAddr = immidiate;
        numOfWords = 2;
      }
      else if ((words[1] = isDirect(working)).AREdata.are != 3) {
        sourceAddr = direct;
        numOfWords = 2;
      }
      else if ((words[1] = isRegister(working)).AREdata.are != 3) {
        sourceAddr = directReg;
        numOfWords = 2;
      }
      else
        error(1);
      working = skipBlanks(working);
      if (*working != ',')
        error(0);
      else
        working = skipBlanks(working + 1);
      if ((words[2] = isDirect(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, direct, sourceAddr, op, immidiate, immidiate);
        numOfWords++;
      }
      else if ((words[2] = isRegister(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, directReg, sourceAddr, op, immidiate, immidiate);
        if (sourceAddr == directReg) {
          words[1].registerAddress.destination = words[2].registerAddress.source;
        }
        else
          numOfWords++;
      }
      else if ((words[2] = isImmidiate(working)).AREdata.are != 3 && op == cmp) {
        words[0] = makeInstruction(Absolute, immidiate, sourceAddr, op, immidiate, immidiate);
        numOfWords++;
      }
      else
        error(0);
    }
    else if (op == lea) {
      working = skipBlanks(working+3);
      if ((words[1] = isDirect(working)).AREdata.are != 3)
        numOfWords = 2;
      else
        error(1);
      working = skipBlanks(working);
      if (*working != ',')
        error(0);
      else
        working = skipBlanks(working + 1);
      if ((words[2] = isDirect(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, direct, direct, op, immidiate, immidiate);
        numOfWords++;
      }
      else if ((words[2] = isRegister(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, directReg, direct, op, immidiate, immidiate);
        numOfWords++;
      }
      else
        error(0);
    }
    else if (op == not || op == clr || op == inc || op == dec || op == dec) { /*third group no source addressing, direct or register destination addressing*/
      working = skipBlanks(working+3);
       if ((words[1] = isDirect(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, direct, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else if ((words[1] = isRegister(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, directReg, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else
        error(1);
    }
    else if (op == prn) {
      working = skipBlanks(working+3);
      if ((words[1] = isImmidiate(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, immidiate, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else if ((words[1] = isDirect(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, direct, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else if ((words[1] = isRegister(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, directReg, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else
        error(1);
    }
    else if (op == jmp || op == bne || op == jsr) {
      working = skipBlanks(working+3);
      sourceAddr = immidiate;
      if ((words[1] = isDirect(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, direct, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else if ((numOfWords = isJmpWParams(working, words, op)))
        ;
      else if ((words[1] = isRegister(working)).AREdata.are != 3) {
        words[0] = makeInstruction(Absolute, directReg, immidiate, op, immidiate, immidiate);
        numOfWords = 2;
      }
      else
        error(1);
    }

    if (skipBlanks(working) != '\0')
      error(1);
    if (labelInstruction != NULL)
      labelInstruction -> adress = IC;
    for (i = 0; i < numOfWords; i++) {
      insertinstructionWord(words[i]);
      if (i == 0)
        updateLine(currentLine);
    }
  }
}

word makeAdressWord(ARE Are, registers *regDest, registers *regSource, unsigned int *address){
  word wrd;
  wrd.AREaddress.are = Are;
  if (address == NULL){
    if (regDest != NULL)
      wrd.registerAddress.destination = *regDest;
    else
      wrd.registerAddress.destination = 0;
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
  }
  return myNum;
}

char *Word2CommaSlash(word wrd){
  char *comSlWord = (char*)malloc(15*sizeof(char));
  int i;
  for (i = 13; i >= 14; i--){
    if (wrd.dword.DATA % 2)
      *(comSlWord + i) = '1';
    else
      *(comSlWord + i) = '0';
    wrd.dword.DATA /= 2;
  }
  *(comSlWord + 14) = '\0';
  return comSlWord;
}

word makeDataWords(ARE *Are, signed int num){
  word newWord;
  if (Are==NULL) {
    if (num < 0)
      newWord.dword.DATA = (unsigned int)(TwosComplement((unsigned int) -num,14).fourteenBit);
    else
      newWord.dword.DATA = num;
  }
  else {
    newWord.AREdata.are = *Are;
    if (num < 0)
      newWord.AREdata.DATA = (unsigned int)(TwosComplement((unsigned int) -num,12).twelveBit);
    else
      newWord.AREdata.DATA = num;
  }
  return newWord;
}

opCode whatOpCode(char *str){
  char op[4];
  int i;
  for (i = 0; i < 3; i++)
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
  else if (!strcmp(op, "sto")) {
    if (*(str+3) == 'p')
      return stop;
    else
      return notAnOp;
  }
  else
    return notAnOp;
}

char *skipBlanks(char *str){
  char *output = str;
  for(;isblank((int) *output) && *output != '\0';output++)
    ;
  return output;
}

word makeInstruction(ARE Are, Addressing dest, Addressing source, opCode opcode, Addressing param1, Addressing param2){
  word wrd;
  wrd.instruction.are              = Are;
  wrd.instruction.destAddressing   = dest;
  wrd.instruction.sourceAddressing = source;
  wrd.instruction.op               = opcode;
  wrd.instruction.param1           = param1;
  wrd.instruction.param2           = param2;
  return wrd;
}

void updateLine(lines *currentLine) {
  if (currentLine -> memType == ICline) {
    currentLine -> instWord = currentInst;
    currentLine -> position = IC;
  }
  else if (currentLine -> memType == DCline) {
    currentLine -> instWord = currentData;
    currentLine -> position = DC;
  }
}

void updateLineList(lines *head){
  if (head -> memType == DCline)
    head -> position += IC + AddressBase;
  else
    head -> position += AddressBase;
  if (head -> next != NULL)
    updateLineList(head -> next);
}

unsigned char hasDirect(void *instWrdAdd) {
  instructionWord WRD = (((wordList*)instWrdAdd) -> Word).instruction;
  unsigned char numOfNames = 0;
  if (WRD.destAddressing == direct || WRD.destAddressing == jmpWparam)
    numOfNames++;
  if (WRD.sourceAddressing == direct)
    numOfNames++;
  if (WRD.param1 == direct)
    numOfNames++;
  if (WRD.param2 == direct)
    numOfNames++;
  return numOfNames;
}

void updateAddress(label *nameLabel, void *instWrdAdd, unsigned int pos) {
  wordList *lblWRD = (wordList*)instWrdAdd;
  while ((lblWRD -> Word).AREaddress.are != 3)
    lblWRD = lblWRD -> next;
  if (nameLabel -> addId == external) {
    if (nameLabel -> adress == -1) {
      nameLabel -> adress = pos;
      lblWRD -> Word = makeAdressWord(External, NULL, NULL, NULL);
    }
    else {
      label *nextNameLabel;
      if ((nextNameLabel = findLabel(nameLabel -> name, nameLabel)))
        updateAddress(nextNameLabel, lblWRD, pos);
      else {
        label *newLabel = (label*)malloc(sizeof(label));
        strncpy(newLabel -> name, nameLabel -> name, MAX_NAME_LENGTH);
        newLabel  -> adress = pos;
        newLabel  -> id     = nameLabel -> id;
        newLabel  -> addId  = nameLabel -> addId;
        newLabel  -> next   = nameLabel -> next;
        nameLabel -> next   = newLabel;
        lblWRD -> Word = makeAdressWord(External, NULL, NULL, NULL);
      }
    }
  }
  else if (nameLabel -> addId == noneAdd) 
    lblWRD -> Word = makeAdressWord(Relocatable, NULL, NULL, &pos);
}

void updateLabelAddress(label *head) {
  if (head -> addId == noneAdd){
    if (head -> id == noneData)
      head -> adress += AddressBase;
    else
      head -> adress += IC + AddressBase;
  }
  else
    head -> adress = -1;
  if (head -> next != NULL)
    updateLabelAddress(head -> next);
}

void makeOutputFile(FILE *output){
  wordList *head = instructionHead;
  unsigned int counter = 0;
  char *wrdCS;
  fprintf(output, "   %d      %d\n",IC+1, DC+1);
  while (head -> next != NULL) {
    wordList *temp = head;
    wrdCS = Word2CommaSlash(temp -> Word);
    free(head);
    fprintf(output, "%4d    %s\n", AddressBase+counter, wrdCS);
    head = temp -> next;
    free(wrdCS);
  }
  wrdCS = Word2CommaSlash(head -> Word);
  fprintf(output, "%4d    %s\n", AddressBase+counter, wrdCS);
  free(head);
  free(wrdCS);
}

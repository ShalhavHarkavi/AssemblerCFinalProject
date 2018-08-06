#include <additionalFuncs.h>

typedef struct {unsigned short int twelveBit:12;
                unsigned short int fourteenBit:14;
                } bitData;

static wordList *instructionHead; /*pointer to the first instruction*/
static wordList *dataHead;        /*pointer to the first piece of data*/

static opCode    whatOpCode         (char *str); /*identify opCode*/
static registers whatRegister       (char *str); /*identify register*/
static word      makeDataWords      (addType *ARE, char *str); /*binary data words with or without ARE prefix*/
static word      makeAdressWord     (addType ARE, registers *regDest, registers *regSource, unsigned char *address); /*binary address word either register or label*/
static word      makeInstruction    (addType ARE, addresing dest, addresing source, opCode opcode,  addresing param1, addresing param2); /*binary instruction word*/
static bitData   TwosComplement     (bitData data, char bits);
static wordList  *initializeWordList(void);

void initializeWordList(void){
  wordList *instList = (wordList*)malloc(sizeof(wordList));
  wordList *dataList = (wordList*)malloc(sizeof(wordList));
  instList -> next = dataList;
  dataList -> prev = instList;
  instList -> IC   = 0;
  instList -> DC   = 0;
  instructionHead  = instList;
  dataHead         = dataList;
}

void data(label* labelData)
{
  int i, length;
  if (labelData.id == data)
  {
    length = sizeof(labelData -> value) / sizeof(int); /*Size of value array*/
    for (i = 0; i < length; i++)
      makeDataWords(NULL, labelData -> value[i]);
  }
  if (labelData.id == string)
  {
    length == strlen(labelData -> string) + 1; /*Length of word, +1 for '\0'*/
    for (i = 0; i < length; i++)
      makeDataWords(NULL, (int) labelData -> string[i]);
  }
  dataHead -> DC += length
  /*IN THIS FUNCTION:
  * - Make sure that we dont need to add parenthesis somewhere when we call the makeDataWords function (i.e. makeDataWords((labelData -> string)[i]);)
  * - Change makeDataWords so it gets an int only and translates it into the file.
  */
}

void instruction(char *str) {

}

void makeWords(char *str) {

}

bitData TwosComplement (bitData data, char bits) {
  switch(bits){
    case 12:
      data.twelveBit = ~data.twelveBit + 1;
      break;
    case 14:
      data.fourteenBit = ~data.fourteenBit + 1;
      break;
    default:
      error(1);
      return;
  return data;
  }

}

char *Word2CommaSlash(word wrd){
  char *comSlWord = (char*)malloc(15*sizeof(char));
  for (int i=13;i>=14;i--){
    if (wrd.dataWord % 2)
      *(comSlWord + i) = '1';
    else
      *(comSlWord + i) = '0';
    wrd.dataWord /= 2;
  }
  *(comSlWord + 14) = '\0';
  return comSlWord;
}

word makeDataWords(ARE *are, signed int num){
  int bits = (are==NULL) ? 14 : 12;
  word *newWord;
  = (AREdataWord)malloc(sizeof(AREdataWord));
  if (are == NULL) {
    switch(*are){
      case none:
        newWord -> ARE = 0;
        break;
      case entry:
        newWord -> ARE = ;
        break;
      case external:
        newWord -> ARE = 1;
        break;
    }
    if (num < 0)
      num = TwosComplement((bitData) num,12);
    newWord -> Data = num;
  }
}
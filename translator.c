#include <additionalFuncs.h>

typedef struct {unsigned short int twelveBit:12;
                unsigned short int fourteenBit:14;
                } bitData;

static opCode    whatOpCode     (char *str); /*identify opCode*/
static registers whatRegister   (char *str); /*identify register*/
static word      makeDataWords  (addType *ARE, char *str); /*binary data words with or without ARE prefix*/
static word      makeAdressWord (addType ARE, registers *regDest, registers *regSource, unsigned char *address); /*binary address word either register or label*/
static word      makeInstruction(addType ARE, addresing dest, addresing source, opCode opcode,  addresing param1, addresing param2); /*binary instruction word*/




/*static addresing whatAd(char *str); /*identifies which addresing scheme is in str and returns it's numer*/

void data(label* labelData)
{
  if (labelData.id == data)
  {
    /*NEED TO COMPLETE*/
  }
}

void instruction(char *str) {

}

void makeWords(char *str) {

}

bitData ()

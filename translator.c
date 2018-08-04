#include <additionalFuncs.h>

typedef struct {unsigned short int twelveBit:12;
                unsigned short int fourteenBit:14;
                } bitData;

static opCode    whatOpCode     (char *str); /*identify opCode*/
static registers whatRegister   (char *str); /*identify register*/
static word      makeDataWords  (addType *ARE, char *str); /*binary data words with or without ARE prefix*/
static word      makeAdressWord (addType ARE, registers *regDest, registers *regSource, unsigned char *address); /*binary address word either register or label*/
static word      makeInstruction(addType ARE, addresing dest, addresing source, opCode opcode,  addresing param1, addresing param2); /*binary instruction word*/
static bitData   TwosComplement (bitData data, char bits);



/*static addresing whatAd(char *str); /*identifies which addresing scheme is in str and returns it's numer*/

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
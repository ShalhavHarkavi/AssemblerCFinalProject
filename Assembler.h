/**************************************
 *********** AUG 19 2018 **************
 * By Matan Liber and Shalhav Harkavi *
 * Submitted to Shadi Jazmawi         *
 * Wednesday's group at Allience      *
 **************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32 /*Maxmimum label name length according to the project guidelines +1 for '\0'*/
#define MAX_FILE_NAME_LENGTH 32 /*Maxmimum file name length according to the project guidelines +1 for '\0'*/
#define MAX_LINE_LENGTH 82 /*Maxmimum line length +1 for '\n' +1 for '\0'*/
#define MAX_TYPE_NAME_LENGTH 7 /*Length of the longest type name (string or extern, 6 characters) +1 for '\0'*/
#define MAX_NUMS_IN_DATA_TYPE 37 /*Max line length is 80 characters, minus 5 characters for ".data" and minus 1 for the space between ".data" and the numbers is 74, divided by 2 to get the maximum amount of numbers (single digit numbers with a single comma without spaces in between them) is 37*/
#define MAX_DIGITS_IN_NUMBER 75 /*Max line length is 80 characters, minus 5 characters for ".data" and minus 1 for the space between ".data" and the numbers is 74, plus 1 for '\0' is 75*/
#define true 1
#define false 0
#define AddressBase 100
#define isblank(c) ((!isprint(c) && c != '\0') || c == ' ')

typedef enum ErrorCode{fopenError, syntaxError, nameError, lineLengthError, fileNumError,
                       JmpNotLabel, param1Err, param2Err, expectComma, illegalOpErr,
                       illegalDest, illegalSource, expectParen, expectEOL, EntryError,
                       labelNotFound, externalErr}errorCode;

typedef enum LabelType{noneData, data, string}type; /*Codes for the regular label types (data, string or none)*/

typedef enum AdditionalLabelType{noneAdd, entry, external}addType; /*Codes for the additional label types (entry, external or none)*/

typedef enum {Normal, Warnning, Error} errorCondition;

typedef struct Label /*Label linked list structure*/
{
    char name[MAX_NAME_LENGTH]; /*Label's name*/
    int adress; /*Label's adress*/
    type id; /*Label's type*/
    addType addId; /*Label's additional type*/
    signed int *value; /*Label's numeric value*/
    char *string; /*Label's string value*/
    struct Label *next; /*Label's pinter to next label in list*/
}label;

typedef enum{ICline, DCline, nonMemLine} ICDC;

typedef struct LineMap {unsigned int lineNum;     /*source file line number    */
                        unsigned long int filePos;/*source file line position  */
                        void *instWord;           /*pinter to memory map       */
                        signed int position;      /*address in memory map      */
                        ICDC memType;             /*part of memory map (IC/DC) */
                        struct LineMap *next;
                       } lines;

/*pointer to a new link in the list starting *head at head after the where link*/
lines *addLine(lines *where, lines **head);

int isEqual(char str1[], char str2[]); /*Returns true if str1 and str2 are the same*/

int isLegalName(char str[]); /*Returns true if str is a legal name according to the guidelines of the project*/

int isLabel(char str[]); /*Returns true if str (a line from the input file) includes a label (excludeing extern and entry definitions)*/

char *getLabelName(char str[], lines* line); /*Returns a pointer to the name of the label in str (a line from the input file)*/

type getType(char str[]); /*Returns the type (data, string, none) used in str (a line from the input file)*/

addType getAddType(char str[]); /*Returns the additional type (entry, external, none) used in str (a line from the input file)*/

int *getValue(char str[], type id, lines *line); /*Returns a pointer to an array that stores the numbers in a data type label defined in str (str is aline from the file, returns NULL if label is not data type)*/

char *getString(char str[], type id); /*Returns a pointer to the string defined in str (str is a line from the file, returns NULL if not defined as .string)*/

void error(errorCode errorType, unsigned int location, char *nameERR);

/*public function for non-ARE data words creation*/
void Data(label* labelData, lines *currentLine);

/*public function for instruction words creation*/
void instruction(char *str, label* labelInstruction, lines *currentLine);

int isDataLabel(char str[]); /*Returns true if str (a line from the file) is defining a data label (.data or .string)*/

int isInstructionLabel(char str[]); /*Returns true if str (a line from the file) is defining an instruction label (a label that is defined using one of the 16 instructions)*/

void initializeWordList(void); /*initializes a static wordList in the file     *
                                *translator.c and updates the 2 head pointers  *
                                *for the instruction and data segments         */

void clearWordList(void); /*clears the wordList in translator.c, should be used if *
                       *errors occur before moving to the next file            */

void makeOutputFile(FILE *output);
void updateLineList(lines *head);     /*add IC offset to all DC lines          */
void clearLinesMap(lines *head);      /*clear lines memory mapping list        */
unsigned char hasDirect(void *instWrdAdd);/*# names of labels from word at pos */
int getName(char **line, char Name[]);/*get the next eligible name in line and advance ptr */
label *findLabel(char *str, label *head);
/*return the label with name = str or NULL if can't in the label list head*/

char *skipBlanks(char *str);/*return pointer to first non-blank beginnig of str*/

/*update relevant addresses in the second pass of the assembler*/
void updateAddress(label *nameLabel, void *instWrdAdd, unsigned int pos);
void updateLabelAddress(label *head); /*update addresses for labels linked list*/

/*update entry labels with the address of the corresponding instruction label*/
void updateEntries(label *head, label *current);

/*A void function that frees the memory of the label linked list*/
void destroyLabelList(label* head);

/*Returns true if str (a line from the file) is in the legal length*/
int isLegalLineLength(char str[]);

/* return the error condition for the current line*/
errorCondition getErrCond(void);
/* reset the error condition to normal before starting to process a new file.*/
void resetErrCond(void);

/*create an empty label and return it's pointer*/
label *createLabel(void);
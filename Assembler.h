/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32
#define MAX_FILE_NAME_LENGTH 32
#define MAX_LINE_LENGTH 82
#define MAX_TYPE_NAME_LENGTH 7
#define true 1
#define false 0
#define AddressBase 100
#define isblank(c) ((!isprint(c) && c != '\0') || c == ' ')

typedef enum ErrorCode{fopenError, syntaxError, nameError, lineLengthError, fileNumError/*Add more error types here*/}errorCode;

typedef enum LabelType{noneData, data, string}type;

typedef enum AdditionalLabelType{noneAdd, entry, external}addType;

typedef enum {Normal, Warnning, Error} errorCondition;

typedef struct Label
{
  char name[MAX_NAME_LENGTH];
  int adress;
  type id;
  addType addId;
  signed int *value;
  char *string;
  struct Label *next;
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

int isEqual(char str1[], char str2[]);

int isLegalName(char str[]);

int isLabel(char str[]);

char *getLabelName(char str[]);

type getType(char str[]);

addType getAddType(char str[]);

int *getValue(char str[], type id);

char *getString(char str[], type id);

void error(errorCode errorType);

/*public function for non-ARE data words creation*/
void Data(label* labelData, lines *currentLine);

/*public function for instruction words creation*/
void instruction(char *str, label* labelInstruction, lines *currentLine);

int isDataLabel(char str[]);

int isInstructionLabel(char str[]);


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

/*update all entry labels with the address of the corresponding instruction label*/
void updateEntries(label *head, label *current);

void destroyLabelList(label* head);

int isLegalLineLength(char str[]);

errorCondition getErrCond(void);
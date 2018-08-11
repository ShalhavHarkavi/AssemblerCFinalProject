/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

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
  int adress; /*Relative position of this label's position from the relevant   *
               *type's head i.e the first data or string label will have the   *
               *address 0 as well as the first instruction label this way there*
               *is no need to update data labels when new instruction labels   *
               *are created (which will have a smaller address since all the   *
               *instructions are entered into the output file before the data. */
  type id;
  addType addId;
  signed int *value;
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

void error(char code);

void data(label* labelData); /*public function for non-ARE data words creation*/

/*public function for instruction words creation*/
void instruction(char *str, label* labelInstruction);

int isDataLabel(char str[]);

int isInstructionLabel(char str[]);


void initializeWordList(void); /*initializes a static wordList in the file     *
                                *translator.c and updates the 2 head pointers  *
                                *for the instruction and data segments         */

void clearWordList(wordList *head); /*clears the wordList in translator.c,     *
                                     *should be used with errors before moving *
                                     *to the next file                         */

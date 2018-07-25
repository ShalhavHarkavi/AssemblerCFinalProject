#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32
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
	int value;
	char *string;
	struct Label *next;
}label;

int isEqual(char str1[], char str2[]);

int isLegalName(char str[]);

int isLabel(char str[]);

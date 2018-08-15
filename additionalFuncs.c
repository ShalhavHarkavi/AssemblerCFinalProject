/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"

int isEqual(char str1[], char str2[])
{
	int count1, count2, i;
	for (count1 = 0; str1[count1] != '\0'; count1++);
	for (count2 = 0; str2[count2] != '\0'; count2++);
	if (count1 != count2)
		return false;
	for (i = 0; i < count1; i++)
	{
		if (str1[i] != str2[i])
			return false;
	}
	return true;
}

int isLegalName(char str[])
{
	if (isEqual(str, "r0") == true || isEqual(str, "r1") == true || isEqual(str, "r2") == true || isEqual(str, "r3") == true || isEqual(str, "r4") == true || isEqual(str, "r5") == true || isEqual(str, "r6") == true || isEqual(str, "r7") == true || isEqual(str, "mov") == true || isEqual(str, "cmp") == true || isEqual(str, "add") == true || isEqual(str, "sub") == true || isEqual(str, "not") == true || isEqual(str, "clr") == true || isEqual(str, "lea") == true || isEqual(str, "inc") == true || isEqual(str, "dec") == true || isEqual(str, "jmp") == true || isEqual(str, "bne") == true || isEqual(str, "red") == true || isEqual(str, "prn") == true || isEqual(str, "jsr") == true || isEqual(str, "rts") == true || isEqual(str, "stop") == true || isEqual(str, "data") == true || isEqual(str, "string") == true || isEqual(str, "entry") == true || isEqual(str, "extern") == true || isalpha((int)str[0]) == false)
		return false;
	return true;
}

int isLabel(char str[])
{
	int i;
	char name[MAX_NAME_LENGTH];
	for (i = 0; str[i] != ':' && str[i] != '\0'; i++);
	if (str[i] == '\0')
		return false;
	strncpy(name, str, i);
	if (isLegalName(name) == false)
		return false;
	return true;
}

char *getLabelName(char str[])
{
	int i;
	char *name = (char*)malloc(sizeof(char) * MAX_NAME_LENGTH);
	for (i = 0; str[i] != ':'; i++);
	strncpy(name, str, i);
	return name;
}

type getType(char str[])
{
	int i, j;
	char dataType[7];
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
	if (str[i] == '\0')
	    return noneData;
	i++;
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++);
	strncpy(dataType, (str + i), (j - i));
	if (isEqual(dataType, "string") == true)
		return string;
	if (isEqual(dataType, "data") == true)
		return data;
	return noneData;
}

addType getAddType(char str[])
{
	int i, j;
	char addType[7];
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
	if (str[i] == '\0')
	    return noneAdd;
	i++;
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++);
	strncpy(addType, (str + i), (j - i));
	if (isEqual(addType, "entry") == true)
		return entry;
	if (isEqual(addType, "extern") == true)
		return external;
	return noneAdd;
}

int *getValue(char str[], type id)
{
	int i, j, t, z;
	char *valueChar;
	int *valueArr = (int*)malloc(sizeof(int));
	if (id == noneData || id == string)
		return NULL;
	for (t = 0, z = 0; str[t] != '\0'; t = j, z++)
	{
		if (z > (sizeof(valueArr) / sizeof(int)))
			realloc(valueArr, (sizeof(int) * z));
		for (i = t; !isdigit((int)str[i]); i++);
		for (j = i; str[j] != ',' && str[j] != ' ' && str[j] != '\t'; j++);
		if (str[j] == ',' && str[j + 1] == ',')
		{
			error(syntaxError);
			return 0; /*Placeholder return.*/
		}
		valueChar = (char*)malloc(sizeof(char) * (j - i));
		strncpy(valueChar, (str + i), (j - i));
		valueArr[z] = atoi(valueChar);
	}
	return valueArr;
}

char *getString(char str[], type id)
{
	int i, j;
	char *string;
	for (i = 0; str[i] != '"'; i++);
	i++;
	for (j = i; str[j] != '"'; j++);
	string = (char*)malloc(sizeof(char) * (j - i));
	strncpy(string, (str + i), (j - i));
	return string;
}

int isDataLabel(char str[])
{
	if (isLabel(str) == true)
	{
		int i;
		for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
		if (str[i] == '\0')
			return false;
		return true;
	}
	return false;
}

int isInstructionLabel(char str[])
{
	if (isLabel(str) == true && isDataLabel(str) == false)
		return true;
	return false;
}

void clearLinesMap(lines *head) {
	lines *temp = head;
	if (temp -> next != NULL)
		head = temp -> next;
	free(temp);
}
int getName(char *line, char Name[]) {
	if (isLabel(line)) {
		char *labelName = getLabelName(line);
		line += strlen(labelName);
		free(labelName);
	}
	line = skipBlanks(skipBlanks(line) + 4); /*skip blanks and name of op*/
	if (*line == '\n' || *line == '\0')
		return false;
	else if (*line == '#')
		while (*(line++) != ',')
			if (*line == '\0')
				return false;
	else if (*line == '(')
		return getName(line + 1, Name);
	else {
		int i;
		for (i=0; isalnum((int)*(line + i)); i++)
			;
		if (i < MAX_NAME_LENGTH) {
			strncpy(Name, line, i);
			if (isLegalName(Name)) {
				line++;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return getName(line, Name);
}

/* not very efficient maybe if there's time investigate using a hash table or 
a sorted structure for the labels */
label *findLabel(char *str, label *head) {
	if (!strcmp(head -> name, str))
		return head;
	else if (head -> next == NULL)
		return NULL;
	else
		return findLabel(str, head -> next);
}

void updateEntries(label *head, label *current) {
	if (current -> addId == entry && current -> adress == -1) {
		label *nameLabel = findLabel(current -> name, head);
		while (nameLabel) {
			if (nameLabel -> addId != noneAdd) {
				nameLabel = findLabel(current -> name, nameLabel -> next);
				continue;
			}
			else {
				current -> adress = nameLabel -> adress;
				break;
			}
		}
		if (nameLabel == NULL)
			error(12);
	}
	if (current -> next != NULL)
		updateEntries(head, current -> next);
}


void error(errorCode errorType)
{
	if (errorType == fopenError)
		fprintf(stderr, "AN ERROR OCCURED WHILE TRYING TO OPEN THE INPUT FILE. /*Maybe specify which input file? Need to think about how to do it.*/");
	if (errorType == syntaxError)
		fprintf(stderr, "A SYNTAX ERROR HAS BEEN DETECTED /*Maybe specify line number? Need to think about how to do it.*/.");
}

int isblank(char c) {
	return (c == ' ' || c == '\t') ? 1 : 0;
}

lines *addLine(lines *where, lines **head) {
	lines *newLine;
	if (*head) {
		newLine = (lines*)malloc(sizeof(lines));
		if (where)
			where -> next = newLine;
		else/* if ((*head) -> next)*/ {
			newLine -> next = (*head) -> next;
			(*head) -> next = newLine;
		}
		return newLine;
	}
	else {
		*head = (lines*)malloc(sizeof(lines));
		return *head;
	}
}
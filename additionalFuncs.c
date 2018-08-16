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
	if (isEqual(str, "r0") == true || isEqual(str, "r1") == true || isEqual(str, "r2") == true || isEqual(str, "r3") == true || isEqual(str, "r4") == true || isEqual(str, "r5") == true || isEqual(str, "r6") == true || isEqual(str, "r7") == true || isEqual(str, "mov") == true || isEqual(str, "cmp") == true || isEqual(str, "add") == true || isEqual(str, "sub") == true || isEqual(str, "not") == true || isEqual(str, "clr") == true || isEqual(str, "lea") == true || isEqual(str, "inc") == true || isEqual(str, "dec") == true || isEqual(str, "jmp") == true || isEqual(str, "bne") == true || isEqual(str, "red") == true || isEqual(str, "prn") == true || isEqual(str, "jsr") == true || isEqual(str, "rts") == true || isEqual(str, "stop") == true || isEqual(str, "data") == true || isEqual(str, "string") == true || isEqual(str, "entry") == true || isEqual(str, "extern") == true || isalpha((int)str[0]) == false || strlen(str) > MAX_NAME_LENGTH)
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
	name[i] = '\0';
	if (isLegalName(name) == false)
		return false;
	return true;
}

char *getLabelName(char str[])
{
	int i;
	char *name = NULL;
	char *start = skipBlanks(str);
	for (i = 0; start[i] != ':'; i++);
	i++;
	if (i > MAX_NAME_LENGTH)
		return NULL; /*here put error*/
	name = (char*)malloc(sizeof(char) * i);
	strncpy(name, start, i - 1);
	name[i] = '\0';
	return name;
}

type getType(char str[])
{
	int i, j, typeLength;
	char bigDataType[7];
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
	if (str[i] == '\0')
	    return noneData;
	i++;
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++);
	typeLength = j - i;
	strncpy(bigDataType, (str + i), typeLength);
	bigDataType[typeLength] = '\0';
	char dataType[typeLength + 1];
	strcpy(dataType, bigDataType);
	dataType[typeLength] = '\0';
	if (isEqual(dataType, "string") == true)
		return string;
	if (isEqual(dataType, "data") == true)
		return data;
	return noneData;
}

addType getAddType(char str[])
{
	int i, j, typeLength;
	char bigAddType[7];
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
	if (str[i] == '\0')
	    return noneAdd;
	i++;
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++);
	typeLength = j - i;
	strncpy(bigAddType, (str + i), typeLength);
	bigAddType[typeLength] = '\0';
	char addType[typeLength + 1];
	strcpy(addType, bigAddType);
	addType[typeLength] = '\0';
	if (isEqual(addType, "entry") == true)
		return entry;
	if (isEqual(addType, "extern") == true)
		return external;
	return noneAdd;
}

int *getValue(char str[], type id)
{
	int i, j, t, z, numCount = 0;
	int bigValueArr[41];
	int *valueArr = NULL;
	if (id == noneData || id == string)
		return NULL;
	for (t = 0, z = 0; str[t] != '\0'; t = j, z++)
	{
		char numString[75];
		for (i = t; !isdigit((int)str[i]) && str[i] != '-'; i++);
		for (j = i; str[j] != ',' && str[j] != ' ' && str[j] != '\t' && str[j] != '\0'; j++);
		if (str[j] == ',' && str[j + 1] == ',')
		{
			error(syntaxError);
			return 0;
		}
		strncpy(numString, (str + i), (j - i));
		numString[j-i] = '\0';
		bigValueArr[z] = atoi(numString);
		numCount++;
	}
	valueArr = (int*)malloc(numCount * sizeof(int));
	for (i = 0; i < numCount; i++)
		valueArr[i] = bigValueArr[i];
	return valueArr;
}

char *getString(char str[], type id)
{
	if (id == noneData || id == data)
		return NULL;
	int i, j;
	char *string;
	for (i = 0; str[i] != '"'; i++);
	i++;
	for (j = i; str[j] != '"'; j++);
	string = (char*)malloc(sizeof(char) * (j - i));
	strncpy(string, (str + i), (j - i));
	string[j-i] = '\0';
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
int getName(char **line, char Name[]) {
	if (isInstructionLabel(*line)) {
		char *labelName = getLabelName(*line);
		*line = skipBlanks(*line + strlen(labelName)+1) + 4;
		free(labelName);
	}
	*line = skipBlanks(*line); /*skip blanks and name of op*/
	if (**line == '\n' || **line == '\0')
		return false;
	if (**line == ',')
		*line = skipBlanks(*line +1);
	else if (**line == '#') {
		while (*++*line != ',')
			if (**line == '\0')
				return false;
	}
	else if (**line == '(') {
		*++*line;
		return getName(line, Name);
	}
	else if (**line == '.') {
		*++*line;
		return getName(line, Name);
	}
	else {
		int i;
		for (i=0; isalnum((int)*(*line + i)); i++)
			;
		if (i && i < MAX_NAME_LENGTH) {
			strncpy(Name, *line, i);
			Name[i] = '\0';
			*line += i;
			if (isLegalName(Name))
				return true;
		}
		else
			return false;
	}
	return getName(line, Name);
}

/* not very efficient maybe if there's time investigate using a hash table or 
a sorted structure for the labels */
label *findLabel(char *str, label *head) {
	if (!head)
		return NULL;
	else if (!strcmp(head -> name, str))
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
		fprintf(stderr, "AN ERROR OCCURED WHILE TRYING TO OPEN THE INPUT FILE.\n" /*Maybe specify which input file? Need to think about how to do it.*/);
	if (errorType == syntaxError)
		fprintf(stderr, "A SYNTAX ERROR HAS BEEN DETECTED.\n" /*Maybe specify line number? Need to think about how to do it.*/);
	if (errorType == nameError)
		fprintf(stderr, "AN ILLEGAL NAME HAS BEEN DETECTED.\n");
	if (errorType == lineLengthError)
		fprintf(stderr, "A LINE THAT EXCEEDS THE MAXIMUM LINE LENGTH OF 80 CHARACTERS HAS BEEN DETECTED.\n");
	if (errorType == fileNumError)
		fprintf(stderr, "NO FILES HAVE BEEN ENTERED.\n");
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
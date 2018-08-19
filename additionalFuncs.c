/**************************************
 *********** AUG 19 2018 **************
 * By Matan Liber and Shalhav Harkavi *
 * Submitted to Shadi Jazmawi         *
 * Wednesday's group at Allience      *
 **************************************/

#include "Assembler.h"

int isEqual(char str1[], char str2[]) /*Returns true if str1 and str2 are the same*/
{
	int count1, count2, i;
	for (count1 = 0; str1[count1] != '\0'; count1++); /*Sets the value of count1 to the length of str1*/
	for (count2 = 0; str2[count2] != '\0'; count2++); /*Sets the value of count2 to the length of str2*/
	if (count1 != count2) /*If the length of str1 isn't equal to the length of str2, then the function returns false*/
		return false;
	for (i = 0; i < count1; i++) /*Runs through str1 and str2, and returns false if at any point the character at position i of str1 isn't equal to the character in the same position in str2*/
	{
		if (str1[i] != str2[i])
			return false;
	}
	return true; /*Returns true if the strings are equal*/
}

int isLegalName(char str[]) /*Returns true if str is a legal name according to the guidelines of the project*/
{
	if (isEqual(str, "r0") == true || isEqual(str, "r1") == true || isEqual(str, "r2") == true || isEqual(str, "r3") == true || isEqual(str, "r4") == true || isEqual(str, "r5") == true || isEqual(str, "r6") == true || isEqual(str, "r7") == true || isEqual(str, "mov") == true || isEqual(str, "cmp") == true || isEqual(str, "add") == true || isEqual(str, "sub") == true || isEqual(str, "not") == true || isEqual(str, "clr") == true || isEqual(str, "lea") == true || isEqual(str, "inc") == true || isEqual(str, "dec") == true || isEqual(str, "jmp") == true || isEqual(str, "bne") == true || isEqual(str, "red") == true || isEqual(str, "prn") == true || isEqual(str, "jsr") == true || isEqual(str, "rts") == true || isEqual(str, "stop") == true || isEqual(str, "data") == true || isEqual(str, "string") == true || isEqual(str, "entry") == true || isEqual(str, "extern") == true || isalpha((int)str[0]) == false || strlen(str) > MAX_NAME_LENGTH)
		return false; /*Returns false if str is equal to any illegal name or doesn't follow the other guidelines of the project. Else, returns true*/
	return true;
}

int isLabel(char str[]) /*Returns true if str (a line from the input file) includes a label (excludeing extern and entry definitions)*/
{
	int i;
	char name[MAX_NAME_LENGTH];
	for (i = 0; str[i] != ':' && str[i] != '\0'; i++); /*Runs through the line until encounters either a colon ar '\0' are encountered*/
	if (str[i] == '\0') /*If '\0' is encountered, that means that there is no colon, so the function returns false, because a label-defining line is structured like: LABEL_NAME: .data/.string/inctruction...*/
		return false;
	strncpy(name, skipBlanks(str), i); /*Copies the name of the label from the first character after blank spaces and tabs to a string*/
	name[i] = '\0'; /*Sets the character after the name in the string to '\0'*/
	if (isLegalName(name) == false) /*Checks if the label name is legal. If not, returns falase. Else, returns true*/
		return false;
	return true;
}

char *getLabelName(char str[], lines *line) /*Returns a pointer to the name of the label in str (a line from the input file)*/
{
	int i;
	char *name = NULL; /*Label name pointer that will be returned from function*/
	char *start = skipBlanks(str); /*Points to the first character in the line that isn't a blank space or a tab*/
	for (i = 0; start[i] != ':'; i++); /*Runs through the line until a colon is encountered*/
	i++; /*Advances 1 for '\0'*/
	if (i > MAX_NAME_LENGTH) /*If the length of the name (including '\0') is greater than the maximum legal length of a name (defined by the project guidelines, includeing '\0'), call the error function for a name error and return a NULL pointer*/
	{
		error(nameError, line -> lineNum, NULL);
		return NULL;
	}
	name = (char*)malloc(sizeof(char) * i); /*Allocates memory for the label name so it can be returned from the function, in the size of the name plus '\0'*/
	strncpy(name, start, i - 1); /*Copies the name from the string to the pointer*/
	name[i] = '\0'; /*Adds '\0' to the pointer*/
	return name; /*Returns the label name pointer*/
}

type getType(char str[]) /*Returns the type (data, string, none) used in str (a line from the input file)*/
{
	int i, j, typeLength;
	char bigDataType[MAX_TYPE_NAME_LENGTH]; /*String that can store the longest type name*/
	char *dataType = NULL; /*Type name pointer*/
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++); /*Runs through the line either a dot or '\0' are encountered*/
	if (str[i] == '\0') /*If '\0' is encountered, that means that no type is found in the line, and noneData is returned*/
	    return noneData;
	i++; /*Advances 1 to get to the first letter of the type after the dot*/
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++); /*Runs from the first letter of the type until a black space or a tab is encountered*/
	typeLength = j - i; /*Finds the length of the type name*/
	strncpy(bigDataType, (str + i), typeLength); /*Copies the name of the type to the maximum length string*/
	bigDataType[typeLength] = '\0'; /*Adds '\0' to the maximum lenth type string*/
	dataType = (char*)malloc(sizeof(char)*(typeLength + 1)); /*Allocates memory for the name of the type in the size of the actual type length*/
	strcpy(dataType, bigDataType); /*Copies the type name from the maximum length string to the pointer*/
	dataType[typeLength] = '\0'; /*Adds '\0' to the pointer*/
	if (isEqual(dataType, "string") == true) /*Checks if the type name stored in the pointer is string. If so, frees the pointer's memory and returns type string*/
	{
		free(dataType);
		return string;
	}
	if (isEqual(dataType, "data") == true) /*Checks if the type name stored in the pointer is data. If so, frees the pointer's memory and returns type data*/
	{
		free(dataType);
		return data;
	}
	free(dataType); /*Frees the pointer's memory*/
	return noneData; /*Returns the only left option that isn't data or string, noneData*/
}

addType getAddType(char str[]) /*Returns the type (entry, external, none) used in str (a line from the input file)*/
{
	int i, j, typeLength;
	char bigAddType[7]; /*String that can store the longest type name*/
	char *addType; /*Type name pointer*/
	for (i = 0; str[i] != '.' && str[i] != '\0'; i++); /*Runs through the line either a dot or '\0' are encountered*/
	if (str[i] == '\0') /*If '\0' is encountered, that means that no type is found in the line, and noneAdd is returned*/
	    return noneAdd;
	i++; /*Advances 1 to get to the first letter of the type after the dot*/
	for (j = i; str[j] != ' ' && str[j] != '\t'; j++); /*Runs from the first letter of the type until a black space or a tab is encountered*/
	typeLength = j - i; /*Finds the length of the type name*/
	strncpy(bigAddType, (str + i), typeLength); /*Copies the name of the type to the maximum length string*/
	bigAddType[typeLength] = '\0'; /*Adds '\0' to the maximum lenth type string*/
	addType = (char*)malloc((typeLength + 1)*sizeof(char)); /*Allocates memory for the name of the type in the size of the actual type length*/
	strcpy(addType, bigAddType); /*Copies the type name from the maximum length string to the pointer*/
	addType[typeLength] = '\0'; /*Adds '\0' to the pointer*/
	if (isEqual(addType, "entry") == true) /*Checks if the type name stored in the pointer is entry. If so, frees the pointer's memory and returns type entry*/
	{
		free(addType);
		return entry;
	}
	if (isEqual(addType, "extern") == true) /*Checks if the type name stored in the pointer is extern. If so, frees the pointer's memory and returns type external*/
	{
		free(addType);
		return external;
	}
	free(addType); /*Frees the pointer's memory*/
	return noneAdd; /*Returns the only left option that isn't entry or external, noneAdd*/
}

int *getValue(char str[], type id, lines *line) /*Returns a pointer to an array that stores the numbers in a data type label defined in str (str is aline from the file, returns NULL if label is not data type)*/
{
	int i, j, t, z, numCount = 0;
	int bigValueArr[MAX_NUMS_IN_DATA_TYPE]; /*Array that can store the maximum amount of numbers in a line that is defined by .data without a label*/
	int *valueArr = NULL; /*Pointer to actual array that will be returned and will have memory allocated later in the size that is actually the number of numbers in the line*/
	if (id == noneData || id == string) /*If the type in the line isn't data, returns a NULL pointer*/
		return NULL;
	for (t = 0, z = 0; str[t] != '\0'; t = j, z++) /*Runs through the entire line, with t signifing the position in str and z the position in the number array:*/
	{
		char numString[MAX_DIGITS_IN_NUMBER]; /*Creates a string that can contain a single number in maximum length in string form*/
		for (i = t; !isdigit((int)str[i]) && str[i] != '-'; i++); /*Runs through the line until a number or negative sign is found*/
		for (j = i; str[j] != ',' && str[j] != ' ' && str[j] != '\t' && str[j] != '\0'; j++); /*Runs through the rest of the line until the number ends*/
		if (str[j] == ',' && str[j + 1] == ',') /*Checks if there are two commas with no numbers between them. If so, calls a syntax error*/
		{
			error(syntaxError, line -> lineNum, NULL);
			return NULL;
		}
		strncpy(numString, (str + i), (j - i)); /*copies the number from the line to the number string*/
		numString[j-i] = '\0'; /*Adds '\0' to the end of the number string*/
		bigValueArr[z] = atoi(numString); /*Adds the number from the string to the number array using atoi()*/
		numCount++; /*Adds 1 to the number counter*/
	}
	valueArr = (int*)malloc((numCount + 1) * sizeof(int)); /*Allocates memory for the number array pointer in the size of the number counter + 1 for the number counter*/
	valueArr[0] = numCount; /*Sets the first number in the number array the amount of numbers*/
	for (i = 0; i < numCount; i++) /*Copies the numbers from the big number array the the pointer array*/
		valueArr[i+1] = bigValueArr[i];
	return valueArr; /*Returns the number array pointer*/
}

char *getString(char str[], type id) /*Returns a pointer to the string defined in str (str is a line from the file, returns NULL if not defined as .string)*/
{
	int i, j;
	char *string; /*A pointer to the string that will have memory allocated for it later in the length of the actual string*/
	if (id == noneData || id == data) /*If the id in the line is not string, return a NULL pointer*/
		return NULL;
	for (i = 0; str[i] != '"'; i++); /*Runs through the line until it finds a quotation mark (start of a string)*/
	i++; /*Advances 1 to get to the start of the string after the quotation mark*/
	for (j = i; str[j] != '"'; j++); /*Runs in the line from the start of the string until it finds the closing quotation mark*/
	string = (char*)malloc(sizeof(char) * (j - i + 1 /*NEED TO CHECK IF +1 IS OK*/)); /*Allocates memory for the string pointer*/
	strncpy(string, (str + i), (j - i)); /*Copies the string from the line to the pointer*/
	string[j - i] = '\0'; /*Adds '\0' to the end of the string pointer*/
	return string; /*Returns the string pointer*/
}

int isDataLabel(char str[]) /*Returns true if str (a line from the file) is defining a data label (.data or .string)*/
{
	if (isLabel(str) == true) /*Checks if the line is defining a label*/
	{
		int i;
		for (i = 0; str[i] != '.' && str[i] != '\0'; i++); /*Runs through the line until a dot or '\0' are encountered*/
		if (str[i] == '\0') /*If no dot is encountered, that means there is no data declaration (no .data or .string), so returns false. else, returns true*/
			return false;
		return true;
	}
	return false; /*Returns false if the line doesn't define a label*/
}

int isInstructionLabel(char str[]) /*Returns true if str (a line from the file) is defining an instruction label (a label that is defined using one of the 16 instructions)*/
{
	if (isLabel(str) == true && isDataLabel(str) == false) /*Checks if the line is defining a label, but is not a data label. If so, returns true. Else, returns false*/
		return true;
	return false;
}

int getName(char **line, char Name[]) /**/
{
	if (isInstructionLabel(*line)) {
		char *labelName = getLabelName(*line, NULL);
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
		++*line;
		return getName(line, Name);
	}
	else if (**line == '.') {
		++*line;
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
			error(EntryError, 0, current -> name);
	}
	if (current -> next != NULL)
		updateEntries(head, current -> next);
}

lines *addLine(lines *where, lines **head) {
	lines *newLine;
	if (*head) {
		newLine = (lines*)malloc(sizeof(lines));
		if (where)
			where -> next = newLine;
		else {
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

int isLegalLineLength(char str[]) /*Returns true if str (a line from the file) is in the legal length*/
{
	int i;
	for (i = 0; str[i] != '\0'; i++);
	if (i == MAX_LINE_LENGTH && strchr(str, '\n') == NULL) /*Checks if there is no occurence of '\n' in the line str (strchr returns NULL if there is no occurence of the character). If so, the line is too long (fgets is supposed to read the newline in addition to the rest of the line), and returns false. Ele, returns true*/
		return false;
	return true;
}

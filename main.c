/**************************************
 *********** AUG 19 2018 **************
 * By Matan Liber and Shalhav Harkavi *
 * Submitted to Shadi Jazmawi         *
 * Wednesday's group at Allience      *
 **************************************/

#include "Assembler.h"

static int assembler(char *fileName); /*The main assembling function*/

/*The second pass adds the addresses of the labels used in the file and        *
  identifies the extternals and entries.                                       */
static void secondPass(FILE *input, label *head, lines *linesMapHead);

void secondPass(FILE *input, label *head, lines *linesMapHead) {
/*the functions runs through the lines of the input file (or more precisely    *
  the instruction words created for lines containning instructions) and looks  *
  for those lines where labels are used as parameters either as direct or as   *
  jumps. When one is found the corresponding label is searched for in the label*
  list created in the first pass and it's adress is inserted with the correct  *
  ARE bit to the next available place in the memory map.                       */
	if (linesMapHead -> memType == ICline) {
		unsigned char numOfNames;
		numOfNames = hasDirect(linesMapHead -> instWord);
		if (numOfNames) {
			char *line = (char*)malloc(sizeof(char)*MAX_LINE_LENGTH);
			char *lineP = line;
			int i;
			fseek(input, linesMapHead -> filePos, SEEK_SET);
			fgets(line, MAX_LINE_LENGTH, input);
			for (i = 0; i < numOfNames; i++) {
				char Name[MAX_NAME_LENGTH];
				label *nameLabel;
				if (getName(&line, Name)) {
					nameLabel = findLabel(Name, head);
					while (nameLabel) {
						if (nameLabel -> addId == entry) {
							nameLabel = findLabel(Name, nameLabel -> next);
							continue;
						}
						else {
							updateAddress(nameLabel, linesMapHead -> instWord, linesMapHead -> position);
							break;
						}
					}
					if (nameLabel == NULL)
						error(labelNotFound, linesMapHead -> lineNum, Name);
				}
			}
			free(lineP);
		}
	}
	if (linesMapHead -> next != NULL)
		secondPass(input, head, linesMapHead -> next);
	updateEntries(head, head);
}

int assembler(char *fileName) 
{
	FILE *input; /*File pointer for the input*/
	label *head = NULL, *temp = NULL; /*Pointers for the linked lists of the labels*/
	char line[MAX_LINE_LENGTH], inputName[MAX_FILE_NAME_LENGTH]; /*String to store the next line in the file and the names of the files plus their extension*/
	unsigned int lineCounter; /*A variable that saves the number of the current line in the input file.*/
	lines *currentLine = NULL; /*pointer to the current line info in the lines DS*/
	lines *linesMapHead = NULL; /*head of the lines list*/
	strncpy(inputName, fileName, MAX_FILE_NAME_LENGTH - 4); /*Copying the input file's name to its corresponding string*/
	strcat(inputName, ".as"); /*Adding the input file's extension to its corresponding string*/
	input = fopen(inputName, "r"); /*Opening the input file using its string in read mode*/
	if (input == NULL) /*Checking if the input file cannot open. If not, it prints an error*/
	{
		error(fopenError, 0, fileName);
		return 0;
	}
	head = createLabel(); /*Allocating memory to the head of the label list*/
	temp = head; /*Setting the value of the temp label pointer to the head of the list, so it can continue through the list without losing the value of head*/
	initializeWordList(); /*initialize the translator for new file processing*/
	resetErrCond(); /*reset the error mechanism for new file processing*/
	for (lineCounter = 1;fgets(line, MAX_LINE_LENGTH, input) != NULL; lineCounter++) /*Running through the input file until EOF is reached (fgets == NULL), while simultaneously increasing the line counter's value*/
	{
		char lineName[MAX_NAME_LENGTH]; /*Creating a string to store the name of the label that might be in the line*/
		/*Checking if the length of the current line from the file is too long. If it is, it calls for an error*/
		if (isLegalLineLength(line) == false)
		{
			char c;
			while ((c = fgetc(input)) != '\n' && c != EOF); /*Procceeds through the too-long-line so it gets to the next*/
			if (skipBlanks(line)[0] != ';') /*If the first character in the line after blanks and tabs isn't a semicolon (the line isn't a comment):*/
				error(lineLengthError, lineCounter, NULL); /*Calls for an error*/
		}
		/*------------add a line to the lines metadata list------------------------*/
		currentLine = addLine(currentLine, &linesMapHead); 
		currentLine -> lineNum = lineCounter; 
		currentLine -> filePos = ftell(input) - strlen(line); 
		currentLine -> memType = nonMemLine; 
		currentLine -> instWord = NULL; 
		currentLine -> position = -1; 
		/*-------------------------------------------------------------------------*/
		if (skipBlanks(line)[0] == ';' || *skipBlanks(line) == '\0') /*Checking if the first character after blank spaces and tabs is a semicolon. If it is, it skips the line, because a line starting with a semicolon is a comment, and must be ignored by the assembler*/
			continue;
		else if (skipBlanks(line)[0] == '.') /*Checking if the first character after blank spaces and tabs is a dot. If it is, it means that the line either declares data without a label, an entry that is entry or external, or has a syntax error*/
		{
			addType addid;
			if (getType(line) != noneData) /*Checking if the type used in the line is string or data. If it is, it creates a temporary label with no name, copies the data in the line to it, creates data lines for the output file and stores them and then resets the temporary label*/
			{
				currentLine -> memType = DCline;
				temp -> id = getType(line); /*Getting the type of data and putting it in the temporary label*/
				temp -> addId = noneAdd; /*Setting the additional id of the temporary label to none, because it cannot be both data and entry/extern if not a label*/
				temp -> value = getValue(line, temp -> id, currentLine); /*Setting the numeric .data value of the temporary label (NULL if .string)*/
				temp -> string = getString(line, temp -> id); /*Setting the .string value of the temporary label (NULL if .data)*/
				if (temp -> string || temp -> value)
					Data(temp, currentLine); /*Creating data lines for the output file of the value stored in the temporary label*/
				temp -> id = 0;
				temp -> addId = 0;
				temp -> value = NULL;
				temp -> string = NULL;
			}
			else if ((addid = getAddType(line)) != noneAdd) /*Else, checking if the additional type used in the line is entry or extern. If it is, it creates a label that stores the name and additional type used in the line and moves temp one step forward in the label list*/
			{
				char *linep = (char*)line;
				if (getName(&linep, lineName))
				{
					strcpy(temp -> name, lineName);
					temp -> addId = addid;
					temp -> next = createLabel();
					temp = temp -> next;
				}
				else
					error(nameError, lineCounter, NULL);
			}
			else /*Else, the only left option is a syntax error, so it calls a syntax error using the function error.*/
				error(syntaxError, lineCounter, NULL);
		}
		else if (isDataLabel(line) == true) /*Else, checking if the line is a declaration of a data label (a label that is declared using .data or .string). If it is:*/
		{
			currentLine -> memType = DCline; /*this line goes into the data portion of the memory*/
			strcpy(lineName, getLabelName(line, currentLine)); /*Copying the label name from the line*/
			if (isLegalName(lineName) == false) /*Checking if the name is legal according to the guidelines of the project. If not, calls an illegal name error*/
				error(nameError, lineCounter, NULL);
			strcpy(temp -> name, lineName); /*Copying the label name to the label structure*/
			temp -> id = getType(line); /*Setting the type of the label from the line (data, string, none)*/
			temp -> addId = getAddType(line); /*Setting the additional type of the label from the line (entry, extern, none)*/
			temp -> value = getValue(line, temp -> id, currentLine); /*Setting the numeric .data value of the label (NULL if .string)*/
			temp -> string = getString(line, temp -> id); /*Setting the .string value of the label (NULL if .data)*/
			temp -> next = createLabel(); /*Allocating memory for the next label in the list*/
			if (temp -> string || temp -> value) {
				Data(temp, currentLine); /*Creates a word/multiple words for the stored data*/
				temp = temp -> next; /*Sets the label pointer from the current one to the next one in the list*/
			}
			else {
				(temp -> name)[0] = '\0';
				temp -> id = 0;
				temp -> addId = 0;
				temp -> value = NULL;
				temp -> string = NULL;
			}
		}
		else if (isInstructionLabel(line) == true) /*Else, checking if the line is a declaration of an instruction label (a label that is declared using one of the 16 instructions). If it is:*/
		{
			currentLine -> memType = ICline; /*this line goes into the instruction portion of the memory*/
			strcpy(lineName, getLabelName(line, currentLine)); /*Copying the label name from the line*/
			if (isLegalName(lineName) == false) /*Checking if the name is legal according to the guidelines of the project. If not, calls an illegal name error*/
				error(nameError, lineCounter, NULL);
			strcpy(temp -> name, lineName); /*Copying the label name to the label structure*/
			instruction(line+strlen(lineName)+1, temp, currentLine); /*Creating instruction lines for the output file and storing them*/
			temp -> next = createLabel(); /*Allocating memory for the next label in the list*/
			temp = temp -> next; /*Sets the label pointer from the current one to the next one in the list*/
		}
		else /*Else, the only optin left is a pure instruction line:*/
		{
			currentLine -> memType = ICline; /**/
			instruction(line, NULL, currentLine); /*Creating instruction lines for the output file and storing them*/
		}
	}
	if (getErrCond() != Error) {
		FILE *output, *entries, *externals; /*output, entries and externals files*/
		char outputName[MAX_FILE_NAME_LENGTH], entriesName[MAX_FILE_NAME_LENGTH], externalsName[MAX_FILE_NAME_LENGTH]; /*names of the input and output files plus their extension*/
		updateLineList(linesMapHead); /*update addresses for the words created to the memorymap*/
		updateLabelAddress(head); /*update addresses for the words created to the memorymap*/
		secondPass(input, head, linesMapHead); /*Calling a function that passes over the file for a second time so it can get adresses and merge entry labels with their corresponding defined labels*/
		strncpy(outputName, fileName, MAX_FILE_NAME_LENGTH - 4); /*Copying the output file's name to its corresponding string*/
		strcat(outputName, ".ob"); /*Adding the output file's extension to its corresponding string*/
		strncpy(entriesName, fileName, MAX_FILE_NAME_LENGTH - 5); /*Copying the entries file's name to its corresponding string*/
		strcat(entriesName, ".ent"); /*Adding the entries file's extension to its corresponding string*/
		strncpy(externalsName, fileName, MAX_FILE_NAME_LENGTH - 5); /*Copying the externals file's name to its corresponding string*/
		strcat(externalsName, ".ext"); /*Adding the externals file's extension to its corresponding string*/
		output = fopen(outputName, "w"); /*Opening the output file using its string in writing mode so the contents of it are rewritten every time the assembler is activated on the input file*/
		entries = fopen(entriesName, "w"); /*Opening the entries file using its string in writing mode so the contents of it are rewritten every time the assembler is activated on the input file*/
		externals = fopen(externalsName, "w"); /*Opening the externals file using its string in writing mode so the contents of it are rewritten every time the assembler is activated on the input file*/
		temp = head; /*Setting the temporary label pointer to the head of the label list*/
		while (temp != NULL) /*While the temporary pointer isn't NULL (meaning - running from the head of the list to its end):*/
		{
			if (temp -> addId == external) {/*Checking if the current label is an external label. If so, prints the name and adress into the externals file*/
				if (temp -> adress < 0)
					error(externalErr, 0, temp -> name);
				else
					fprintf(externals, "%s\t%d\n", temp -> name, temp -> adress);
			}
			if (temp -> addId == entry) /*Checking if the current label is an entry label. If so, prints the name and adress into the entries file*/
				fprintf(entries, "%s\t%d\n", temp -> name, temp -> adress);
			temp = temp -> next; /*Setting the label pointer from the current label to the next one in the list*/
		}
		if (entries == NULL) /*Checking if the entries file pointers is NULL (file is empty). If it is, the file is closed and removed*/
		{
			fclose(entries);
			remove(entriesName);
		}
		if (externals == NULL) /*Checking if the externals file pointers is NULL (file is empty). If it is, the file is closed and removed*/
		{
			fclose(externals);
			remove(externalsName);
		}
		makeOutputFile(output); 
		if (getErrCond() == Warnning)
			printf("there were warnings but the object file was still created.\n");
	}
	fclose(input); /*Closing the input file*/
	destroyLabelList(head); /*Destroying the label linked list to free space*/
	clearWordList();
	clearLinesMap(linesMapHead);
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	if (argc == 1) /*Checking if the number of arguments entered into the command line is 1 (only ./assembler). If so, it calls a file number error*/
	{
		error(fileNumError, 0, NULL);
		return 0;
	}
	for (i = 1; i < argc; i++) /*Starts from the command line argument in position 1 (the one after ./assembler), and calls the assembler function on it*/
	{
		printf("NOW PROCESSING FILE : %s.AS\n",argv[i]);
		assembler(argv[i]);
	}
	return 0;
}

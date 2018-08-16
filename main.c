/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"
static void secondPass(FILE *input, label *head, lines *linesMapHead);

void secondPass(FILE *input, label *head, lines *linesMapHead) {
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
						error(12);
				}
				else
					error(10);
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
	FILE *input, *output, *entries, *externals;
	label *head = NULL, *temp = NULL;
	char line[MAX_LINE_LENGTH], inputName[MAX_FILE_NAME_LENGTH], outputName[MAX_FILE_NAME_LENGTH], entriesName[MAX_FILE_NAME_LENGTH], externalsName[MAX_FILE_NAME_LENGTH];
	unsigned int lineCounter;
	lines *currentLine = NULL;
	lines *linesMapHead = NULL;
	strncpy(inputName, fileName, MAX_FILE_NAME_LENGTH - 4);
	strcat(inputName, ".as");
	strncpy(outputName, fileName, MAX_FILE_NAME_LENGTH - 4);
	strcat(outputName, ".ob");
	strncpy(entriesName, fileName, MAX_FILE_NAME_LENGTH - 5);
	strcat(entriesName, ".ent");
	strncpy(externalsName, fileName, MAX_FILE_NAME_LENGTH - 5);
	strcat(externalsName, ".ext");
	input = fopen(inputName, "r");
	if (input == NULL)
	{
		error(fopenError);
		return 0; /*Need to think about if to use return so it skips to the next file or use exit so it just stops everything*/
	}
	output = fopen(outputName, "w");
	entries = fopen(entriesName, "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	externals = fopen(externalsName, "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	head = (label*)malloc(sizeof(label));
	temp = head;
	initializeWordList();
	for (lineCounter = 1;fgets(line, MAX_LINE_LENGTH, input) != NULL; lineCounter++)
	{
		/*if (line[MAX_LINE_LENGTH - 2] != '\n')
		{
			error(lineLengthError);
			return 0;
		}*/
		char lineName[MAX_NAME_LENGTH];
		currentLine = addLine(currentLine, &linesMapHead);
		currentLine -> lineNum = lineCounter;
		currentLine -> filePos = ftell(input) - strlen(line) -0;
		currentLine -> memType = nonMemLine;
		currentLine -> instWord = NULL;
		currentLine -> position = -1;
		if (skipBlanks(line)[0] == ';' || *skipBlanks(line) == '\0')
			continue; /*So it skips comments. Need to check if the syntax is right (meaning if 'continue' is the right command).*/
		else if (skipBlanks(line)[0] == '.')
		{
			addType addid;
			if (getType(line) != noneData)
			{
				temp -> id = getType(line);
				temp -> addId = noneAdd;
				temp -> value = getValue(line, temp -> id);
				temp -> string = getString(line, temp -> id);
				Data(temp, currentLine);
				temp = NULL;
				free(temp);
				temp = (label*)malloc(sizeof(label));
			}
			else if ((addid = getAddType(line)) != noneAdd)
			{
				/*char* skipBlanksLine = skipBlanks(line);
				char* lastLetter;
				int charNum;
				if (*skipBlanksLine == '\0')
				{
					error(syntaxError);
					return 0;
				}
				lastLetter = skipBlanksLine;
				for (charNum = 0; isalnum(lastLetter); lastLetter++, charNum++);
				strncpy(lineName, skipBlanksLine, charNum);
				lineName[charNum] = '\0';
				if (isLegalName(lineName) == true)*/
				char *linep = (char*)line;
				if (getName(&linep, lineName))
				{
					strcpy(temp -> name, lineName);
					temp -> addId = addid;
					temp -> next = (label*)malloc(sizeof(label));
					temp = temp -> next;
				}
				else
				{
					error(nameError);
					return 0;
				}
			}
			else
			{
				error(syntaxError);
				return 0;
			}
		}
		else if (isDataLabel(line) == true)
		{
			currentLine -> memType = DCline;
			strcpy(lineName, getLabelName(line));
			if (isLegalName(lineName) == false)
			{
				error(nameError);
				return 0;
			}
			strcpy(temp -> name, lineName); /*Maybe get rid of the line befire that, and replace this line with strncpy with n being MAX_NAME_LENGTH?*/
			temp -> id = getType(line);
			temp -> addId = getAddType(line);
			temp -> value = getValue(line, temp -> id);
			temp -> string = getString(line, temp -> id);
			temp -> next = (label*)malloc(sizeof(label));
			Data(temp, currentLine); /*Creates a word/multiple words for the stored datas*/
			temp = temp -> next;
		}
		else if (isInstructionLabel(line) == true)
		{
			currentLine -> memType = ICline;
			strcpy(lineName, getLabelName(line));
			strcpy(temp -> name, lineName); /*Maybe get rid of the line befire that, and replace this line with strncpy with n being MAX_NAME_LENGTH?*/
			instruction(line+strlen(lineName)+1, temp, currentLine);
			temp -> next = (label*)malloc(sizeof(label));
			temp = temp -> next;
		}
		else
		{
			currentLine -> memType = ICline;
			instruction(line, NULL, currentLine);
		}
	}
	updateLineList(linesMapHead);
	updateLabelAddress(head);
	secondPass(input, head, linesMapHead); 
	fclose(input);
	temp = head;
	while (temp != NULL)
	{
		if (temp -> addId == external)
			fprintf(externals, "%s\t%d\n", temp -> name, temp -> adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		if (temp -> addId == entry)
			fprintf(entries, "%s\t%d\n", temp -> name, temp -> adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		temp = temp -> next;
	}
	if (entries == NULL)
	{
		fclose(entries);
		remove(strcat(fileName, ".ent"));
	}
	if (externals == NULL)
	{
		fclose(externals);
		remove(strcat(fileName, ".ext"));
	}
	makeOutputFile(output);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		error(fileNumError);
		return 0;
	}
	int i;
	for (i = 1; i < argc; i++)
		assembler(argv[i]);
	return 0;
}

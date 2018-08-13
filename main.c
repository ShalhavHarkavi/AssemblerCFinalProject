/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"
static void secondPass(FILE *input, label *head, lines *linesMapHead);

void secondPass(FILE *input, label *head, lines *linesMapHead) {
	unsigned char numOfNames;
	numOfNames = hasDirect(linesMapHead -> instWord);
	if (numOfNames) {
		char line[MAX_LINE_LENGTH];
		int i;
		fseek(input, linesMapHead -> filePos, SEEK_SET);
		fgets(line, MAX_LINE_LENGTH, input);
		for (i = 0; i < numOfNames; i++) {
			char Name[MAX_NAME_LENGTH];
			label *nameLabel;
			if (getName(line, Name)) {
				nameLabel = head;
				while (nameLabel) {
					nameLabel = findLabel(Name, nameLabel);
					if (nameLabel -> addId == entry)
						continue;
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
	}
	if (linesMapHead -> next != NULL)
		secondPass(input, head, linesMapHead -> next);
	updateEntries(head, head);
}

int assembler(char *fileName)
{
	char line[MAX_LINE_LENGTH], lineName[MAX_NAME_LENGTH];
	FILE *input, *output, *entries, *externals;
	label *head = NULL, *temp = NULL;
	unsigned int lineCounter = 0;
	lines *linesMapHead = (lines*)malloc(sizeof(lines));
	lines *currentLine = linesMapHead;
	input = fopen(strcat(fileName, ".as"), "r");
	if (input == NULL)
	{
		error(fopenError);
		return 0; /*Need to think about if to use return so it skips to the next file or use exit so it just stops everything*/
	}
	output = fopen(strcat(fileName, ".ob"), "w");
	entries = fopen(strcat(fileName, ".ent"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	externals = fopen(strcat(fileName, ".ext"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	head = (label*)malloc(sizeof(label));
	temp = head;
	while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
	{
		currentLine -> lineNum = lineCounter;
		currentLine -> filePos = ftell(input) - strlen(line) -1;
		if (line[0] == ';')
			continue; /*So it skips comments. Need to check if the syntax is right (meaning if 'continue' is the right command).*/
		else if (isLabel(line) == true)
		{
			strcpy(lineName, getLabelName(line));
			strcpy(temp -> name, lineName); /*Maybe get rid of the line befire that, and replace this line with strncpy with n being MAX_NAME_LENGTH?*/
			temp -> id = getType(line);
			temp -> addId = getAddType(line);
			temp -> value = getValue(line, temp -> id);
			temp -> string = getString(line, temp -> id);
			temp -> next = (label*)malloc(sizeof(label));
			if (temp -> addId == noneAdd) {
				lines *newLine = (lines*)malloc(sizeof(lines));
				currentLine -> memType = DCline;
				Data(temp, currentLine); /*Creates a word/multiple words for the stored datas*/
				currentLine -> next = newLine;
				newLine -> next = NULL;
				currentLine = newLine;
			}
			temp = temp -> next;
		}
		else if (isInstructionLabel(line) == true)
		{
			lines *newLine = (lines*)malloc(sizeof(lines));
			currentLine -> memType = ICline;
			strcpy(lineName, getLabelName(line));
			strcpy(temp -> name, lineName); /*Maybe get rid of the line befire that, and replace this line with strncpy with n being MAX_NAME_LENGTH?*/
			instruction(line+strlen(lineName)+1, temp, currentLine);
			temp -> next = (label*)malloc(sizeof(label));
			temp = temp -> next;
			currentLine -> next = newLine;
			newLine -> next = NULL;
			currentLine =  newLine;
		}
		else
		{
			lines *newLine = (lines*)malloc(sizeof(lines));
			currentLine -> memType = ICline;
			instruction(line, NULL, currentLine);
			currentLine -> next = newLine;
			newLine -> next = NULL;
			currentLine = newLine;
		}
		lineCounter++;
	}
	updateLineList(linesMapHead);
	updateLabelAddress(head);
	secondPass(input, head, linesMapHead);
	fclose(input);
	temp = head;
	while (temp != NULL)
	{
		if (temp -> addId == external)
			fprintf(externals, "%s\t%d", temp -> name, temp -> adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		if (temp -> addId == entry)
			fprintf(entries, "%s\t%d", temp -> name, temp -> adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
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
	int i;
	for (i = 1; i <= (argc - 1); i++)
		assembler(argv[i]);
	return 0;
}

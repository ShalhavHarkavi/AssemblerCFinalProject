/***********************************
* By Matan Liber and Shalhav Harkavi
***********************************/

#include "additionalFuncs.h"

int assembler(char *fileName)
{
	char line[MAX_LINE_LENGTH/*Placeholder, will find solution for dynamic sized line later.*/], lineName[MAX_NAME_LENGTH];
	FILE* input, output, entries, externals;
	label *head = NULL, temp = NULL;
	input = fopen(strcat(fileName, ".as"), "r");
	if (input == NULL)
	{
		fprintf(stderr, "FAILED TO OPEN THE FILE NAMED %s.as\n", fileName);
		return 0; /*Return instead of exit() because maybe we should just skip files that don't work and try to assemble the ones that do, as return will just skip the current not-working file and move on, and exit() will stop the program entirely if I'm not wrong.*/
	}
	output = fopen(strcat(fileName, ".ob"), "w");
	entries = fopen(strcat(fileName, ".ent"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	externals = fopen(strcat(fileName, ".ext"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	head = (label*)malloc(sizeof(label));
	temp = head;
	while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
	{
		if (isLabel(line) == true)
		{
			name = getLabelName(line);
			temp -> name = lineName;
			temp.id = getType(line);
			temp.addId = getType(line);
			temp -> value = getValue(line, head.id);
			temp -> string = getString(line, head.id);
			temp -> next = (label*)malloc(sizeof(label));
			data(temp); /*Creates a word/multiple words for the stored datas*/
			temp = temp -> next;
		}
		else if (isInstructionLabel(line) == true)
		{
			/*isInstructionLabel: do it by if not data label*/
			name = getLabelName(line);
			temp -> name = lineName;
			instruction(line, temp);
			temp -> next = (label*)malloc(sizeof(label));
			temp = temp -> next;
		}
		else
			instruction(line, NULL);
	}
	temp = head;
	while (temp != NULL)
	{
		if (temp.addId == external)
			fprintf(externals, "%s\t%d", temp -> name, temp.adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		if (temp.addId == entry)
			fprintf(entries, "%s\t%d", temp -> name, temp.adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		temp = temp -> next;
	}
	if (entries == NULL)
	{
		fclose(entries);
		remove(entries);
	}
	if (externals == NULL)
	{
		fclose(externals);
		remove(externals);
	}
}

int main(int argc, char *argv[])
{
	/*NEED HERE TO ACTIVATE ASSEMBLER UNTIL FINISHED WITH ARGV BY SENDING THE NAMES ONE BY ONE*/
}

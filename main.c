#include "additionalFuncs.h"

int assembler(char *fileName)
{
  char line[4096/*Placeholder, will find solution for dynamic sized line later.*/], lineName[MAX_NAME_LENGTH];
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
  externals = fopen(strcat(fileName, ".ent"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
  head = (label*)malloc(sizeof(label));
  while (fgets(line, 4096, input) != NULL)
  {
    if (isLabel(line) == true)
    {
      temp = head;
      name = getLabelName(line);
      while (temp != NULL)
      {
        if (isEqual(temp -> name, lineName) == true)
        {
          fprintf(stderr, "THE LABEL NAMED %s IS DEFINED MORE THAN ONCE.\n", labelName);
          return 0; /*Using return as placeholder for now.*/
        }
      }
      head -> name = lineName;
      /*NEED TO GET ADRESS HERE.*/
      head.id = getType(line);
      head.addId = getType(line);
      head -> value = getValue(line, head.id);
      head -> string = getString(line, head.id);
      head -> next = (label*)malloc(sizeof(label));
      head = head -> next;
    }
  }
}

int main(int argc, char *argv[])
{
  /*NEED HERE TO ACTIVATE ASSEMBLER UNTIL FINISHED WITH ARGV BY SENDING THE NAMES ONE BY ONE*/
}

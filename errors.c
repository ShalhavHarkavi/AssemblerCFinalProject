/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/
/*This module is responsible for error handeling and cleanup.                 */

#include "Assembler.h"

static errorCondition errorCond ; /*state variable for error condition */

errorCondition getErrCond(void) {
  return errorCond;
}

void resetErrCond (void) {
  errorCond = Normal;
}


void error(errorCode errorType, unsigned int location, char* nameERR)
{
  if (errorType == fopenError) {
    errorCond = Error;
    fprintf(stderr, "AN ERROR OCCURED WHILE TRYING TO OPEN THE INPUT FILE: %s.\n",nameERR);
  }
  else if (errorType == syntaxError) {
    fprintf(stderr, "A SYNTAX ERROR HAS BEEN DETECTED IN LINE #%d.\n", location);
    errorCond = Error;
  }
  else if (errorType == nameError) {
    fprintf(stderr, "AN ILLEGAL NAME HAS BEEN DETECTED IN LINE #%d.\n", location);
    errorCond = Error;
  }
  else if (errorType == lineLengthError) {
    fprintf(stderr, "LINE #%d EXCEEDS THE MAXIMUM LINE LENGTH OF 80 CHARACTERS.\n", location);
    errorCond = Error;
  }
  else if (errorType == fileNumError) {
    fprintf(stderr, "NO FILES HAVE BEEN ENTERED.\n");
    errorCond = Error;
  }
  else if (errorType == JmpNotLabel) {
    fprintf(stderr, "NOT A VALID DESTINATION FOR JMP, BNE, JSR WITH PARAMETERS IN LINE #%d.\n", location);
    errorCond = Error;
  }
  else if (errorType == param1Err) {
    fprintf(stderr, "PARAMETER 1 IS NOT A LEGAL WORD IN LINE #%d\n.", location);
    errorCond = Error;
  }
  else if (errorType == param2Err) {
    fprintf(stderr, "PARAMETER 2 IS NOT A LEGAL WORD IN LINE #%d\n.", location);
    errorCond = Error;
  }
  else if (errorType == expectComma) {
    fprintf(stderr, "A COMMA IS EXPECTED BETWEEN PARAMETERS IN LINE #%d\n", location);
    errorCond = Error;
  }
  else if (errorType == illegalDest) {
    fprintf(stderr, "DESTINATION IS ILLEGAL IN LINE #%d\n", location);
    errorCond = Error;
  }
  else if (errorType == illegalSource) {
    fprintf(stderr, "SOURCE IS ILLEGAL IN LINE #%d\n", location);
    errorCond = Error;
  }
  else if (errorType == expectParen) {
    fprintf(stderr, "EXPECTED ')' IN LINE #%d\n", location);
    errorCond = Error;
  }
  else if (errorType == expectEOL) {
    fprintf(stderr, "THERE SHOULDN'T BE ANYTHING AFTER THE COMMAND IN LINE #%d\n", location);
    errorCond = Warnning;
  }
  else if (errorType == EntryError) {
    fprintf(stderr, "AN ENTRY WAS DECLARED BUT NEVER DEFINED: '%s'\n", nameERR);
    errorCond = Error;
  }
  else if (errorType == labelNotFound) {
    fprintf(stderr, "A LABEL '%s' WAS USED BUT NEVER DEFINED IN LINE #%d\n", nameERR, location);
    errorCond = Error;
  }
}

void destroyLabelList(label* head) /*A void function that frees the memory of the label linked list*/
{
    if (head == NULL) /*Stops if current label is NULL (end of linked list)*/
        return;
    destroyLabelList(head -> next); /*Calls the function on the necxt label*/
    if (head -> value)
      free(head -> value);
    if (head -> string)
      free(head -> string);
    free(head); /*Frees the memory of the current label*/
}


void clearLinesMap(lines *head) /* run through the list of lines metadata and clear it.*/
{
  lines *temp = head;
  if (temp -> next != NULL)
    head = temp -> next;
  free(temp);
}

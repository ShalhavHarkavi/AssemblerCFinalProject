/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/
/*This module is responsible for error handeling and cleanup.                 */

#include "Assembler.h"

static errorCondition errorCond ; /*state variable for error condition */

errorCondition getErrCond(void);


void error(errorCode errorType, lines line)
{
	int lineNumber;
	if (line)
		lineNumber = line -> lineNum;
	else
		lineNumber = 0;
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
	if (errorType == JmpNotLabel)
		fprintf(stderr, "Not a valid destination for jmp, bne, jsr with parameters\n");
}

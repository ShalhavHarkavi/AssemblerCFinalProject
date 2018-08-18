/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/
/*This module is responsible for error handeling and cleanup.                 */

#include "Assembler.h"

static errorCondition errorCond ; /*state variable for error condition */

errorCondition getErrCond(void);


void error(errorCode errorType, char *location)
{
	if (errorType == fopenError)
		fprintf(stderr, "AN ERROR OCCURED WHILE TRYING TO OPEN THE INPUT FILE: %s.\n",location);
	if (errorType == syntaxError)
		fprintf(stderr, "A SYNTAX ERROR HAS BEEN DETECTED IN LINE #%s.\n", location);
	if (errorType == nameError)
		fprintf(stderr, "AN ILLEGAL NAME HAS BEEN DETECTED IN LINE #%s.\n", location);
	if (errorType == lineLengthError)
		fprintf(stderr, "A LINE THAT EXCEEDS THE MAXIMUM LINE LENGTH OF 80 CHARACTERS HAS BEEN DETECTED.\n");
	if (errorType == fileNumError)
		fprintf(stderr, "NO FILES HAVE BEEN ENTERED.\n");
	if (errorType == JmpNotLabel)
		fprintf(stderr, "NOT A VALID DESTINATION FOR JMP, BNE, JSR WITH PARAMETERS IN LINE #%s.\n", location);
	if (errorType == param1Err)
		fprintf(stderr, "PARAMETER 1 IS NOT A LEGAL WORD IN LINE #%s\n.", location);
	if (errorType == param2Err)
		fprintf(stderr, "PARAMETER 2 IS NOT A LEGAL WORD IN LINE #%s\n.", location);
	if (errorType == expectComma)
		fprintf(stderr, "A COMMA IS EXPECTED BETWEEN PARAMETERS IN LINE #%s\n", location);
	
}

/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/
/*This module is responsible for error handeling and cleanup.                 */

#include "Assembler.h"

static errorCondition errorCond ; /*state variable for error condition */

errorCondition getErrCond(void);


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
		fprintf(stderr, "A LINE THAT EXCEEDS THE MAXIMUM LINE LENGTH OF 80 CHARACTERS HAS BEEN DETECTED.\n");
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
	else if (errorType == expectParen) {
		fprintf(stderr, "THERE SHOULDN'T BE ANYTHING AFTER THE COMMAND IN LINE #%d\n", location);
		errorCond = Warnning;
	}
	else if (errorType == EntryError) {
		fprintf(stderr, "AN ENTRY WAS DECLARED BUT NEVER DEINED: '%s'\n", nameERR);
		errorCond = Error;
	}
	else if (errorType == labelNotFound) {
		fprintf(stderr, "A LABEL '%s' WAS USED BUT NEVER DEFINED IN LINE #%d\n", nameERR, location);
		errorCond = Error;
	}



}

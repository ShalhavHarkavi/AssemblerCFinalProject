int isEqual(char str1[], char str2[])
{
	int count1, count2, i;
	for (count1 = 0; str1[count1] != '\0'; count1++);
	for (count2 = 0; str2[count2] != '\0'; count2++);
	if (count1 != count2)
		return false;
	for (i = 0; i < count1; i++)
	{
		if (str1[i] != str2[i])
			return false;
	}
	return true;
}

int isLegalName(str[])
{
	if (isEqual(str, "r0") == true || isEqual(str, "r1") == true || isEqual(str, "r2") == true || isEqual(str, "r3") == true || isEqual(str, "r4") == true || isEqual(str, "r5") == true || isEqual(str, "r6") == true || isEqual(str, "r7") == true || isEqual(str, "mov") == true || isEqual(str, "cmp") == true || isEqual(str, "add") == true || isEqual(str, "sub") == true || isEqual(str, "not") == true || isEqual(str, "clr") == true || isEqual(str, "lea") == true || isEqual(str, "inc") == true || isEqual(str, "dec") == true || isEqual(str, "jmp") == true || isEqual(str, "bne") == true || isEqual(str, "red") == true || isEqual(str, "prn") == true || isEqual(str, "jsr") == true || isEqual(str, "rts") == true || isEqual(str, "stop") == true || isEqual(str, "data") == true || isEqual(str, "string") == true || isEqual(str, "entry") == true || isEqual(str, "extern") == true || isalpha(str[0]) == false)
		return false;
	return true;
}

int isLabel(char str[])
{
	int i;
	char name[MAX_NAME_LENGTH];
	for (i = 0; str[i] != ':' || str[i] != '\0'; i++);
	if (str[i] == '\0')
		return false;
	strncpy(name, str, i);
	if (isLegalName(name) == false)
		return false;
	return true;
}

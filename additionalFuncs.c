/***********************************
***********************************/

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

char *getLabelName(char str[])
{
	int i;
	char *name = (char*)malloc(sizeof(char) * MAX_NAME_LENGTH);
	for (i = 0; str[i] != ':'; i++);
	strncpy(name, str, i);
	return name;
}

type getType(char str[])
{
	int i, j;
	char type[7];
	for (i = 0; str[i] != '.'; i++);
	i++;
	for (j = i; str[j] != ' ' || str[j] != '\t'; j++);
	strncpy(type, (str + i), (j - i));
	if (isEqual(type, "string" == true))
		return string;
	if (isEqual(type, "data" == true))
		return data;
	return none;
}

addType getAddType(char str[])
{
	int i, j;
	char addType[7];
	for (i = 0; str[i] != '.'; i++);
	i++;
	for (j = i; str[j] != ' ' || str[j] != '\t'; j++);
	strncpy(addType, (str + i), (j - i));
	if (isEqual(type, "entry") == true)
		return entry;
	if (isEqual(type, "extern") == true)
		return external;
	return none;
}

int *getValue(char str[], type id)
{
	int i, j, t, z, counter = 0;
	char *valueChar;
	int *valueArr = (int*)malloc(sizeof(int));
	if (id = none || id = string)
		return (int)NULL;
	for (t = 0, z = 0; str[t] == '\0'; t = j, z++)
	{
		if (z > (sizeof(valueArr) / sizeof(int)))
			realloc(valuArr, (sizeof(int) * z));
		for (i = t; !isdigit(str[i]); i++);
		for (j = i; str[j] != ',' || str[j] != ' ' || str[j] != '\t'; j++);
		if (str[j] == ',' && str[j + 1] == ',')
		{
			fprintf(stderr, "SYNTAX ERROR\n");
			return 0; /*Placeholder return.*/
		}
		valueChar = (char*)malloc(sizeof(char) * (j - i));
		strncpy(valueChar, (str + i), (j - i));
		valueArr[z] = atoi(valueChar);
	}
	return valueArr;
}

char *getString(char str[], type id)
{
	int i, j;
	char *string;
	for (i = 0; str[i] != '"'; i++);
	i++;
	for (j = i; str[j] != '"'; j++);
	string = (char*)malloc(sizeof(char) * (j - i));
	strncpy(string, (str + i), (j - i));
	return string;
}

int isDataLabel(char str[])
{
	int i;
	for (i = 0; str[i] != '.' || str[i] != '\0'; i++);
	if (str[i] == '\0')
		return false;
	return true;
}

int isInstructionLabel(char str[])
{
	if (isLabel(str) == true && isDataLabel(str) == false)
		return true;
	return false;
}

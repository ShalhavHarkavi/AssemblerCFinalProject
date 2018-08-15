# AssemblerFinalProject
- שגיאות ופלט: מעבר על כל מקום בעבודה שבו יכולה להיווצר שגיאה ולזרוק שם שגיאה
- לסדר את כל הקבצים הנוספים למיין (additionalFuncs וכו')
- לעבור ולבדוק שהכל עובד עם דוגמאות
- לתעד הכל
- לוודא שהקוד לא מתייחס להערות

int *getValue(char str[], type id)
{
	int i, j, t, z, numCount;
	char *bigValueArr[41];
	int *valueArr = NULL;
	if (id == noneData || id == string)
		return NULL;
	for (t = 0, z = 0; str[t] != '\0'; t = j, z++)
	{
		for (i = t; !isdigit((int)str[i]); i++);
		for (j = i; str[j] != ',' && str[j] != ' ' && str[j] != '\t'; j++);
		if (str[j] == ',' && str[j + 1] == ',')
		{
			error(syntaxError);
			return 0;
		}
		strncpy(bigValueArr[z], (str + i), (j - i));
	}
	bigValueArr[z] = '\0';
	for (numCount = 0; bigValueArr[numCount] != '\0'; numCount++);
	valueArr = (int*)malloc(sizeof(int) * numCount);
	for (i = 0; i < numCount; i++)
		valueArr[i] = atoi(bigValueArr[i]);
	return valueArr;
}

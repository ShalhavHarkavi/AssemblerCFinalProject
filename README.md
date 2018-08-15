# AssemblerFinalProject
- שגיאות ופלט: מעבר על כל מקום בעבודה שבו יכולה להיווצר שגיאה ולזרוק שם שגיאה
- לסדר את כל הקבצים הנוספים למיין (additionalFuncs וכו')
- לעבור ולבדוק שהכל עובד עם דוגמאות
- לתעד הכל
- לוודא שהקוד לא מתייחס להערות

# Main: if start of line is '.', new version with data handling (need line handling):
else if (skipBlanks(line)[0] == '.')
		{
			if (getType(line) != noneData)
			{
				temp -> name = NULL;
				temp -> id = getType(line);
				temp -> addId = noneAdd;
				temp -> value = getValue(line, temp -> id);
				temp -> string = getString(line, temp -> id);
				Data(temp, /*Lines?*/);
				temp = NULL;
				free(temp);
				temp = (label*)malloc(sizeof(label));
			}
			else if (getAddType(line) != noneAdd)
			{
				char* skipBlanksLine = skipBlanks(line);
				char* lastLetter;
				int charNum;
				for (; *skipBlanksLine != ' ' && *skipBlanksLine != '\t' && *skipBlanksLine != '\0'; skipBlanksLine++);
				if (*skipBlanksLine == '\0')
				{
					error(syntaxError);
					return 0;
				}
				skipBlanksLine = skipBlanks(skipBlanksLine);
				lastLetter = skipBlanksLine;
				for (charNum = 0; *lastLetter != ' ' && *lastLetter != '\t' && *lastLetter != '\0'; lastLetter++, charNum++);
				strncpy(lineName, skipBlanksLine, charNum);
				if (isLegalName(lineName) == true)
				{
					search = head;
					while (search != NULL)
					{
						if (isEqual(search -> name, lineName) == true && getAddType(line) == entry)
							break;
						search = search -> next;
					}
					if (search == NULL)
					{
						search = temp;
						strcpy(search -> name, lineName);
					}
					search -> id = getType(line);
					search -> addId = getAddType(line);
					temp -> next = (label*)malloc(sizeof(label));
					temp = temp -> next;
				}
			}
		}

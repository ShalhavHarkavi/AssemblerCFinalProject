# AssemblerFinalProject
- שגיאות ופלט: מעבר על כל מקום בעבודה שבו יכולה להיווצר שגיאה ולזרוק שם שגיאה
- לסדר את כל הקבצים הנוספים למיין (additionalFuncs וכו')
- לעבור ולבדוק שהכל עובד עם דוגמאות
- לתעד הכל
- לוודא שהקוד לא מתייחס להערות

# Command line output (errors and warnings as of August 13th, 20:30)
gcc -c -ansi -Wall -pedantic translator.c
gcc -c -ansi -Wall -pedantic main.c 
gcc -c -ansi -Wall -pedantic additionalFuncs.c 
additionalFuncs.c: In function ‘updateEntries’:
additionalFuncs.c:188:3: warning: suggest parentheses around assignment used as truth value [-Wparentheses]
   while (nameLabel = findLabel(current -> name, head)) {
   ^
additionalFuncs.c: In function ‘getValue’:
additionalFuncs.c:91:4: warning: ignoring return value of ‘realloc’, declared with attribute warn_unused_result [-Wunused-result]
    realloc(valueArr, (sizeof(int) * z));
    ^
additionalFuncs.c: In function ‘findLabel’:
additionalFuncs.c:183:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
gcc -o assembler main.o additionalFuncs.o translator.o

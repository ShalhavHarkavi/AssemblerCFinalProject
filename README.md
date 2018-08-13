# AssemblerFinalProject
- שגיאות ופלט: מעבר על כל מקום בעבודה שבו יכולה להיווצר שגיאה ולזרוק שם שגיאה
- לסדר את כל הקבצים הנוספים למיין (additionalFuncs וכו')
- לעבור ולבדוק שהכל עובד עם דוגמאות
- לתעד הכל
- לוודא שהקוד לא מתייחס להערות

# Command line output (errors and warnings aws of August 13th, 18:54)
main.c: In function ‘secondPass’:
main.c:15:3: error: ‘for’ loop initial declarations are only allowed in C99 or C11 mode
   for (int i = 0; i < numOfNames; i++) {
   ^
main.c:15:3: note: use option -std=c99, -std=gnu99, -std=c11 or -std=gnu11 to compile your code
main.c: In function ‘assembler’:
main.c:78:5: error: conversion to non-scalar type requested
     lines *newLine = (lines)malloc(sizeof(lines));
     ^
main.c:78:5: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
main.c:93:4: error: conversion to non-scalar type requested
    lines *newLine = (lines)malloc(sizeof(lines));
    ^
main.c:93:4: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
main.c:102:4: error: conversion to non-scalar type requested
    lines *newLine = (lines)malloc(sizeof(lines));
    ^
main.c:102:4: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
main.c:44:16: warning: variable ‘output’ set but not used [-Wunused-but-set-variable]
  FILE *input, *output, *entries, *externals;
                ^
makefile:5: recipe for target 'main.o' failed
make: *** [main.o] Error 1

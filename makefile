assembler: translator.o main.o additionalFuncs.o errors.o
	gcc -o assembler main.o additionalFuncs.o translator.o

main.o: main.c Assembler.h
	gcc -c -ansi -Wall -pedantic main.c 

additionalFuncs.o: additionalFuncs.c Assembler.h
	gcc -c -ansi -Wall -pedantic additionalFuncs.c 

translator.o: translator.c Assembler.h
	gcc -c -ansi -Wall -pedantic translator.c

errors.o: errors.c Assembler.h
	gcc -c -ansi -Wall -pedantic errors.c

clean: 
	rm -f *.o assembler

debug: translatord.o maind.o additionalFuncsd.o errorsd.o
	gcc -o assembler translatord.o maind.o additionalFuncsd.o

maind.o: main.c Assembler.h
	gcc -c -ansi -Wall -pedantic main.c -o maind.o -ggdb

additionalFuncsd.o: additionalFuncs.c Assembler.h
	gcc -c -ansi -Wall -pedantic additionalFuncs.c -o additionalFuncsd.o -ggdb

translatord.o: translator.c Assembler.h
	gcc -c -ansi -Wall -pedantic translator.c -o translatord.o -ggdb

errorsd.o: errors.c Assembler.h
	gcc -c -ansi -Wall -pedantic errors.c -o errorsd.o -ggdb

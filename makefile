assembler: translator.o main.o additionalFuncs.o
	gcc -o assembler main.o additionalFuncs.o translator.o

main.o: main.c additionalFuncs.h
	gcc -c -ansi -Wall -pedantic main.c 

additionalFuncs.o: additionalFuncs.c additionalFuncs.h
	gcc -c -ansi -Wall -pedantic additionalFuncs.c 

translator.o: translator.c additionalFuncs.h
	gcc -c -ansi -Wall -pedantic translator.c

clean: 
	rm -f *.o assembler

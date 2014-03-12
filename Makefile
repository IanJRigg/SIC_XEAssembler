#Ian Rigg, Richard Valles, Chris Andaya, Arron McCarter\
masc1258\
Prog 2: Makefile\
CS530, Spring 2014\
Team Florida\

#Makefile
TORM = file_parser.o driver.o opcodetab.o driver
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

sicxeas:	sicxeas.o file_parser.o opcodetab.o
		${CC} ${CCFLAGS} -o sicxeas sicxeas.o file_parser.o opcodetab.o

sicxeas.o:	sicxe_asm.cpp sicxe_asm.h symtab.h symtab.cc symtab_exception.h 
		${CC} ${CCFLAGS} -c sicxe_asm.cpp
			
fileparser.o:	file_parser.cc file_parser.h file_parse_exception.h
		${CC} ${CCFLAGS} -c file_parser.cc
				
opcodetab.o:	opcodetab.cc opcodetab.h opcode_error_exception.h
		${CC} ${CCFLAGS} -c opcodetab.cc
				
clean:	
	rm -f ${TORM}
#End Makefile

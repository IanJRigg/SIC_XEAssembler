#Ian Rigg, Richard Valles, Chris Andaya, Arron McCarter\
masc1258\
prog 4: Makefile\
CS530, Spring 2014\
Team Florida\

#Makefile
TORM = opcodetab.o sicxe_asm sicxe_asm.o file_parser.o symtab.o
CC = g++
CCFLAGS = -g -O3 -Wall -Wpointer-arith -Wcast-qual -Wwrite-strings 

sicxe_asm:	sicxe_asm.o 
	${CC} ${CCFLAGS} -o sicxe_asm sicxe_asm.o opcodetab.o file_parser.o symtab.o

sicxe_asm.o:	sicxe_asm.cpp sicxe_asm.h opcodetab.o file_parser.o symtab.o 
		${CC} ${CCFLAGS} -c sicxe_asm.cpp
            
opcodetab.o:	opcodetab.cc opcodetab.h opcode_error_exception.h
			${CC} ${CCFLAGS} -c opcodetab.cc                    


symtab.o:	symtab.cc symtab.h symtab_exception.h
		${CC} ${CCFLAGS} -c symtab.cc  
  
file_parser.o:	file_parser.cc file_parser.h file_parse_exception.h
		${CC} ${CCFLAGS} -c file_parser.cc                
clean:
	rm -f ${TORM}

#End Makefile
                        

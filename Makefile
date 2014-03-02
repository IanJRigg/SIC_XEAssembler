#Ian Rigg, Richard Valles, Chris Andaya, Arron McCarter\
masc1258\
prog 2: makefile\
CS530, Spring 2014\
Team Florida\

#Makefile
TORM = file_parser.o driver.o opcodetab.o driver opcodedriver.o opdriver
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

opdriver: opcodetab.o opdriver.o
		${CC} ${CCFLAGS} -o opdriver opcodedriver.o opcodetab.o            

driver:	driver.o file_parser.o
		${CC} ${CCFLAGS} -o driver driver.o file_parser.o
              
driver.o:	driver.cpp
			${CC} ${CCFLAGS} -c driver.cpp

opdriver.o:	opcodedriver.cpp
			${CC} ${CCFLAGS} -c opcodedriver.cpp                        
			
fileparser.o:	file_parser.cc file_parser.h file_parse_exception.h
				${CC} ${CCFLAGS} -c file_parser.cc
				
opcodetab.o:	opcodetab.cc opcodetab.h opcode_error_exception.h
			${CC} ${CCFLAGS} -c opcodetab.cc 


				
clean:	
	rm -f ${TORM}
#End Makefile

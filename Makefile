#Makefile
TORM = file_parser.o driver.o driver
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

driver:	driver.o file_parser.o
		${CC} ${CCFLAGS} -o driver driver.o file_parser.o

driver.o:	driver.cpp
			${CC} ${CCFLAGS} -c driver.cpp
			
fileparser.o:	file_parser.cc file_parser.h file_parse.exception.h
				${CC} ${CCFLAGS} -c file_parser.cc
				
clean:	rm -f ${TORM}
#End Makefile
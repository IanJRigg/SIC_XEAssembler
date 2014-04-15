Checklist for Program 4

	Create the necessary code to execute a second pass over the assembly code.
	
	Need to add a Machine code line for the listing file and the internal vector.
	
	Things to be accounting for:
	
	
	
	
	
Flags:

if the operand has an @ sign: n = 1, i = 0
if the operand has a # sign: n = 0, i = 1
Otherwise n = 1, i = 1

if there is a + in front of the opcode: e = 1, b = 0, p = 0
otherwise e = 0

if the x register is in the operand field, x = 1
otherwise x = 0

if the base variable is set, check to see if the address is within the range
	if yes, p = 1, b = 0
	otherwise, p = 0, b = 1
	
	
	
	
	
	
	
	
	
Robust error handling; the program should never crash.
	If an error occurs, you should terminate your program and print a helpful error message.
	It should include a description of the problem, and the line number in the source file where the error occurred. 
	If an assemly error occurs, you should not output a listing file.
	
PC-relative addressing used where possible

Base-relative addressing

Indexed addressing

Indirect addressing

Immediate operands

Simple forward references

All four instruction formats must be supported

NO floating point operations or data

NO object record files (only the listing file).

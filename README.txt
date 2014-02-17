Program 1:

	Make a simple file parser to break down a simple .asm file into the following four tokens:
		- label
		- opcode
		- operand
		- comments
		
	Methods left to be completed:
		
		- The constructor:
			file_parser(string filename)
				intitialize the vector and the filename
		
		- The destructor
			~file_parser()
				free all of the occupied memory
		
		- void read_file()
			open the file initialized in the constructor
			iterate through the file one line at a time
				for each line, sort out the tokens into the four listed above.
				determine a mistake in the code and throw the file_parse_exception
		
		- string get_token(unsigned int line, unsigned int column);
			return the token at the given line, in the given column
		
		- int size()
			return the value of the vector's size function

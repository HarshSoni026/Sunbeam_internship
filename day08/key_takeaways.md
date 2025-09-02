1 - How to tokenize and parse the command and parameters.
2 - FTP common commands - USER, PASS, PWD.
3 - study the Modular Command handler in C.

-> Tokenizing - braking string into smaller parts i.e. called as tokens based on delimeters that are " ", ",", "\t", etc.(space, commas, tabs)

-> Parse - Taking tokens and understanding their meaning based on protocol/rules, code which read FTP commands sent by a client (like , USER,  PASS, PWD, QUIT, etc)
When you connect to FTP Server, you send commands in plain text:



    Example Command    |    Meaning                               |       Argument

        USER admin       Set username to 'admin'.                          admin
        PASS 1234        Set password to '1234'.                           1234
        PWD              Present working directory.                        none
        PORT 127.0.0.1   Tell server which IP/Port to connect to           127.0.0.1

-> A parser turns the raw string into structured data the program can use.
    "Think of tokenizing as cutting of sentence into words, and parsing as understanding the grammer of the sentences."

->fgets - The fgets() function in C is used to read a string from a specified input stream and store it in a character array. 
It is considered a safer alternative to gets() because it prevents buffer overflows by allowing the user to specify the maximum number of characters to read.

har *fgets(char *str, int n, FILE *stream);

Parameters:
str: A pointer to the character array (buffer) where the read string will be stored.
n: The maximum number of characters to read, including the null terminator (\0). fgets() will read at most n-1 characters.
stream: The input stream to read from. This can be stdin for standard input (keyboard) or a FILE pointer for reading from a file. 


-> command = strtok(input, " ");

strtok() takes the string input and splits it using " " (space) as a delimiter.

It returns the first token (before the first space).



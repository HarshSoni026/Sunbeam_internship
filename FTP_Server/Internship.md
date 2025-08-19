## FTP Server
FTP
# Day04 
***
Task1: Modify the server to print client messages on the server terminal.
Task2: Modify the server to send a welcome message to the client immediately after connection. (Hint: use write() and accept() ).
***
# Day05
3. Modify the server to terminate connection if client sends "exit".

# Day06
ftp_command_parser.c
1. How to read commands from a socket.
2. How to tokenize and parse the command and parameters.
3. FTP Basecs: common commands (USER, PASS, PWD).
4. Structure of a modular commamd handler in C.

Task1: Add case:- insensitive command matching.
Example: use or UsEr should still match USER.

use strcasecmp() for comparison.

Task2: Extract and print only the command arguments (parameters)
Example: Input: USER admin --> Output: command= USER, Argument=admin.

Task3: Store received commands in a history log (in-memoty array).
Expample: Store last 5 commands and print them on request.

# Day08
1. Read opendir(), readdir(), closedir(), from <dirent.h>
2. How directory listing is used in FTP's LIST command.
opendir.c

Task1: Filter only files or only directories
Hint: Use dir->d_type:

Task2: Accept directory path as command-line argument

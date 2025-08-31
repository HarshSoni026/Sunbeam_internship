Today's Study points:
How to read commands from a socket.
How to tokenize and parse the command and parameters.
FTP basics: common commands (USER, PASS, PWD).
Structure of a modular command handler in C.

Today's Task:
Task 1.

Add case-insensitive command matching.
Example: user or UsEr should still match USER.

Use strcasecmp() for comparison.

Task 2.
Extract and print only the command arguments (parameters).
Example:
Input: USER admin → Output: Command=USER, Argument=admin.

Task 3.
Store received commands in a history log (in-memory array).
Example: Store last 5 commands and print them on request.
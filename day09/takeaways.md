- Read opendir(), readdir(), closedir() from <dirent.h>.
synopsis - #include<dirent.h>
-> opendir() ---> open a directory, returns pointer to the directory stream.

     DIR *opendir(const char *name);
-> readdir() ---> read a directory,  The  readdir()  function  returns  a  pointer  to  a dirent structure representing the next directory entry in the directory stream
       pointed to by dirp.  It returns NULL on reaching the end of the directory stream or if an error occurred.
    struct dirent *readdir(DIR *dirp);

-> closedir() ---> close a directory, 

    int closedir(DIR *dirp);
    The closedir() function closes the directory stream associated with dirp.  A successful call to closedir() also closes the underly‐
       ing file descriptor associated with dirp.  The directory stream descriptor dirp is not available after this call.

RETURN VALUE
       The closedir() function returns 0 on success.  On error, -1 is returned, and errno is set appropriately.

- How directory listing is used in FTP’s LIST command.

The FTP LIST command is a fundamental function used to retrieve a directory listing from a remote FTP server. When an FTP client issues the LIST command, the server responds with information about the files and subdirectories within the current working directory on the server.

Requesting Information: 
The client sends the LIST command to the FTP server. This command indicates the client's desire to view the contents of a specific directory on the server (often the current working directory, unless a path is specified).

Server Response: 
The FTP server processes the LIST command and generates a directory listing. The format of this listing can vary slightly between different FTP server implementations, but it typically includes details similar to those provided by a Unix-like ls -l command.

Information Included: 
The directory listing returned by the LIST command usually contains the following information for each file and directory:

Permissions: 
File and directory permissions (e.g., read, write, execute for owner, group, and others).

Number of Links: 
The number of hard links to the file.

Owner: 
The user account that owns the file or directory.

Group: 
The group that owns the file or directory.

Size: 
The size of the file in bytes.

Modification Date/Time: 
The date and time when the file or directory was last modified.

Name: 
The name of the file or directory.

Client-Side Parsing: 
The FTP client receives this raw directory listing data from the server. It is then the client's responsibility to parse this string data and present it to the user in a readable and organized format. This parsing allows the client to extract individual file and directory attributes for display or further processing.
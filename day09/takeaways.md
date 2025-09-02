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


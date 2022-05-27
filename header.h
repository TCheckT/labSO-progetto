#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <fcntl.h>
#include <signal.h>
#include <time.h>
      
// Used to get number of element of an array 
#define SIZEOF(a) ( sizeof a / sizeof a[0] )

/* Function used in registro, padre_treni, processo_treno and server_RBC 
    to read a message sent to the process through pipe or socket */
int receiveFrom(int fd, char *str);

/* Read a /0 terminating sequence of char from a pipe or socket indicated by fd
    and place them as a string in str */
int receiveFrom(int fd, char *str) { 
    int n;
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
    } while (n > 0 && *str++ != 0);
    return (n > 0); /* Return false if end-of-input */
}

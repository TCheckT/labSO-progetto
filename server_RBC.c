#include "header.h"

int main(int argc, char *argv[]) {

    printf("Server starting with map: %s\n", argv[1]);

    //TODO: communication with register to receive itineraries

    //using pipe
    // MAPPA must be the same that registro received
    while(1) {
        printf("Asking for %s itineraries\n", argv[1]);
        sleep(3);
    }




    printf("Setting up socket...\n");
    sleep(2);

    // socket()
    int serverFd, clientFd, serverLen, clientLen;
    struct sockaddr_un serverUNIXAddress; /*Server address */
    struct sockaddr* serverSockAddrPtr; /*Ptr to server address*/
    struct sockaddr_un clientUNIXAddress; /*Client address */
    struct sockaddr* clientSockAddrPtr;/*Ptr to client address*/

    /* ignore children termination */
    signal(SIGCHLD, SIG_IGN);

    serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
    serverLen = sizeof (serverUNIXAddress);
    clientSockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
    clientLen = sizeof (clientUNIXAddress);
    
    // socket()
    serverFd = socket (AF_UNIX, SOCK_STREAM, 0);
    
    // bind()
    serverUNIXAddress.sun_family = AF_UNIX; /* Set domain type */
    strcpy (serverUNIXAddress.sun_path, "authorization"); /* Set name */
    unlink ("authorization"); /* Remove file if it already exists */
    bind (serverFd, serverSockAddrPtr, serverLen);/*Create file*/
    
    // listen()
    listen (serverFd, 5); /* Maximum pending connection length */

    // accept()
    while (1) {/* Loop forever */ /* Accept a client connection */
    printf("Server RBC is waiting for requests from clients\n");
    clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
    if (fork () == 0) { /* Create child to manage request */
        // WORK HERE:


        printf("request detected\n");
        close (clientFd); /* Close the socket */
        exit (/* EXIT_SUCCESS */ 0); /* Terminate */
    } else
        close (clientFd); /* Close the client descriptor */
    }

}
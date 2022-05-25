#include "header.h"

int receiveStage(int fd, char *str);

struct dataArrays{
    int stations[8];
    int segments[16];
};

int main(int argc, char *argv[]) {

    const int numberOfTrains=(strcmp(argv[1], "MAPPA1") == 0) ? 4 : 5;

    printf("Server starting with map: %s\n", argv[1]);

    printf("Server creating pipe with register\n");
    // eventually unlinking pipe (in case it already exists)
    unlink("serverRegisterPipe");
    // (re)creating pipe
    mknod("serverRegisterPipe", S_IFIFO, 0);
    // setting access permission on pipe 
    chmod("serverRegisterPipe", 0660);

    printf("Server opens pipe with register in write mode\n");
    //opening pipe
    int itineraryRequestPipe_fd = open("serverRegisterPipe", O_WRONLY);
    // writing the map's name
    write(itineraryRequestPipe_fd, argv[1], 7);
    // closing pipe
    unlink("serverRegisterPipe");

    printf("Server allocating data\n");
    struct dataArrays dataServer;
    for (int i = 0; i < 8; ++i)     dataServer.stations[i] = 0;
    for (int i = 0; i < 16; ++i)    dataServer.segments[i] = 0;


    printf("Server accepting stations from register\n");
    /*  [x] for i = 0 ... numberOfTrains
    *   [x]     read station from pipe 
    *   [ ]     atoi -> recieved
    *   [ ]     increments station in struct dataserver.station[recieved]
    */
    char station[5];
    for (int i = 0; i < numberOfTrains; ++i)
    {
        mknod("serverRegisterPipe", S_IFIFO, 0);
        chmod("serverRegisterPipe", 0660);
        itineraryRequestPipe_fd = open("serverRegisterPipe", O_RDONLY);
        receiveStage(itineraryRequestPipe_fd, station);
        printf("Server recieved station:%s\n", station);
        unlink("serverRegisterPipe");
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
int receiveStage(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}

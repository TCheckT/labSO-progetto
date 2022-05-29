#include "header.h"

int signalCounter = 0;

void childTerminationHandler(int signum) {
    signalCounter++;
}

/* Function to create files MAx representing tracks segments */
int createTracks();

int main(int argc, char *argv[]) {
    /* Install SIGUSR1 signal receiver, when receive the same number 
        of SIGUSR1 signal */
    signal(SIGUSR1, childTerminationHandler);
    
    char* MAPPA = argv[1];
    char* ETCS = argv[2];

    // create files that represent tracks segments
    if(createTracks()!=0)
        perror("Error: binary segments not created correctly\n");
    printf("padre_treni: MAx files created\n");

    // launch turn_manager
    pid_t turn_manager;

    turn_manager = fork();
    if (turn_manager < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (turn_manager == 0) {
        // exec turn_manager with MAPPA as argument
        printf("padre_treni: exec turn_manager\n");
        execl("./turn_manager", "turn_manager", MAPPA, NULL);
    } 
    /* process movementAuthority continues */

    /* Before creating trains, if ETCS2 mode, padre treni will receive serverRBC pid */
    int clientFd, serverLen, result;
    struct sockaddr_un serverUNIXAddress;
    struct sockaddr* serverSockAddrPtr;

    pid_t serverRBC;

    if (strcmp(ETCS, "ETCS2") == 0) {
        // socket()
        serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
        serverLen = sizeof (serverUNIXAddress);
        //clientFd = socket (AF_UNIX, SOCK_STREAM, 0);
        serverUNIXAddress.sun_family = AF_UNIX; /* Server domain */
        strcpy (serverUNIXAddress.sun_path, "authorization"); /*Server name*/
        clientFd = socket (AF_UNIX, SOCK_STREAM, 0);
        printf("padre_treni: waiting for server to setup socket and accept connection to receive its pid...\n");
        do { /* Loop until a connection is made with the server */
            result = connect (clientFd, serverSockAddrPtr, serverLen);
            if (result == -1){
                printf("padre_treni: ...\n");
                sleep (1); /* Wait and then try again */
            }
        } while (result == -1);

        char serverPid[1000];
        receiveFrom(clientFd, serverPid);
        printf("padre_treni: server_RBC pid received.\n");
        serverRBC = atoi(serverPid);
    }

    /* create the right number of trains according to MAPPA: 
        always declare 5 pid but if MAPPA1 doesn't initialise the last one */
    pid_t train[5];
    int numberOfTrains=(strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;
    
    for(short int i = 0; i < numberOfTrains; i++){ 
       
        train[i] = fork();
       
        if(train[i] < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(EXIT_FAILURE);
        }
        else if(train[i] == 0) {
            // exec a processo_treno with trainNUmber, MAPPA and ETCS as arguments
            char trainNumber[5];
            sprintf(trainNumber, "%d",i+1);
            printf("padre_treni: exec processo_treno T%s\n", trainNumber);
            execl("./processo_treno", "processo_treno", trainNumber, ETCS, NULL);
        }
    }
    
    /* padre_treni waits here until every train finish its mission and send SIGUSR1 signal to padre_treni
        when a SIGUSR1 signal is received the handler function will increment the global variable signalCounter
        when 4 or 5 signals are received, depending on which MAPPA was launched, padre_treni can continue. */
    while(signalCounter < numberOfTrains);

    /* When 4 (or 5) SIGUSR1 have been received, padre_treni terminates children */
    for (short int i = 0; i < numberOfTrains; i++) {
        printf("padre_treni: terminating T%d\n", i+1);
        kill(train[i], SIGKILL);
    }
    
    /* Tell to server_RBC process to terminate with a SIGUSR2 
        server_RBC sent its pid at the beginning */
    if (strcmp(ETCS, "ETCS2") == 0) {
        kill(serverRBC, SIGUSR2);
    }

    /* Terminate process turn_manager before terminating */
    kill(turn_manager, SIGKILL);

    printf("\npadre_treni: process terminated...\n");

    exit(EXIT_SUCCESS);
    return 0;
}

int createTracks() {
    /* Creating all the 16 files that represent tracks segments */
    FILE * file;
    char name[5];

    for (int i = 1; i <= 16; ++i) {   
        // Generating the name of the file MAx with metavariables
        sprintf(name, "MA%d",i);

        // Opening the file
        file = fopen(name, "w");

        // Setting access permissions
        chmod(name , 0666);

        // Writing 0 as first character
        fwrite("0", sizeof(char), 1, file);

        // Closing file
        fclose(file);
    }
    return 0;
}

#include "header.h"

int readMessage (int fd, char *str);

int signalCounter = 0;

void childTerminationHandler(int signum) {
    signalCounter++;
}

int main(int argc, char *argv[]) {
    /* Install SIGUSR1 signal receiver, when receive the same number 
        of SIGUSR1 signal */
    signal(SIGUSR1, childTerminationHandler);
    
    char* MAPPA = argv[1];
    char* ETCS = argv[2];

    // create files that represent tracks segments
    if(createTracks()!=0)
        perror("Error: binary segments not created correctly\n");
    
    //launch turn_manager
    pid_t turn_manager;

    turn_manager = fork();
    if (turn_manager < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (turn_manager == 0) {
        // execute turn_manager
        execl("./turn_manager", "turn_manager", MAPPA, NULL);
    } 
    // padre_treni continues 

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
        do { /* Loop until a connection is made with the server */
            result = connect (clientFd, serverSockAddrPtr, serverLen);
            if (result == -1){
                printf("padre_treni: waiting for server to setup socket and receive its pid...\n");
                sleep (1); /* Wait and then try again */
            }
        } while (result == -1);

        char serverPid[10000];
        readMessage(clientFd, serverPid);
        printf("serverPid: %s\n", serverPid);
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
            // execute a train process 
            char trainNumber[5];
            sprintf(trainNumber, "%d",i+1);
            execl("./processo_treno", "processo_treno", trainNumber, ETCS, NULL);
        }
    }
    // wait for all children to finish
   /* wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    if(numberOfTrains == 5) wait(NULL); */
    /* ATTEMPT TO REALIZE OPTIONAL TASK 2: supposed to use SIGUSR1*/
    while(signalCounter < numberOfTrains);

    /* Kill server_RBC process that sent its pid at the beginning when all trains terminate
        their missions */
    if (strcmp(ETCS, "ETCS2") == 0) {
        kill(serverRBC, SIGUSR2);
    }

    kill(turn_manager, SIGKILL);

    exit(EXIT_SUCCESS);
    return 0;
}

int createTracks() {
    FILE * file; //buffer for file operations
    char name[5];

    for (int i = 1; i <= 16; ++i)
    {   
        //generating the name of the file MAx with metavariables
        sprintf(name, "MA%d",i);

        //opening the file in write-only mode
        file = fopen(name, "w");

        //setting access permissions
        chmod(name , 0666);

        // writing 0 as a first character in every file
        fwrite("0", sizeof(char), 1, file);

        //closing files
        fclose(file);
    }
    return 0;
}

int readMessage (int fd, char *str) {
    /* Read a single ’\0’-terminated line into str from fd */
    int n;
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
    } while (n > 0 && *str++ != '\0');
    return (n > 0); 
}

/* ATTEMPT TO REALIZE OPTIONAL TASK 2: supposed to use SIGUSR1
void trainEndMissionHandler(int signum) {
    
    wait(NULL);
    //printf("Ricevo %d° SIGUSR1!\n", ++signalCounter);
    //sleep(1);
}
*/

#include "header.h"

/* Function for reading a stage received from register through pipe */
int receiveStage(int fd, char *str);
/* Function for reading messages received from trains through pipe */
int readMessage (int fd, char *str);

struct stagesStatus{
    int stations[8];
    int segments[16];
};

struct trainsInfo{
    char name[3];
    char itinerary[10][5];
};

int main(int argc, char *argv[]) {

    const int numberOfTrains=(strcmp(argv[1], "MAPPA1") == 0) ? 4 : 5;

    printf("Server starting with map: %s\n", argv[1]);

    printf("Server initialising its data structures\n");

    /* Data stucture to keep track segments and stations occupation statuses */
    struct stagesStatus status;
    for (int i = 0; i < 8; ++i)     status.stations[i] = 0;
    for (int i = 0; i < 16; ++i)    status.segments[i] = 0;

    /* Arrays of int that will be use to keep track of where the train is along its itinerary */
    int progresses[numberOfTrains];
    for (int i = 0; i < numberOfTrains; ++i)     progresses[i] = 0;

    /* Data structure to save trains itineraries while receiving them from register */
    struct trainsInfo train[5];

    /* Server has to receive all itineraries through pipe from register */
    for (int i = 0; i < numberOfTrains; ++i) {
        char receivedStage[5];

        sprintf(train[i].name, "T%d", i+1);

        printf("Creating pipe between server and register\n");
        unlink("serverRegisterPipe");
        mknod("serverRegisterPipe", S_IFIFO, 0);
        chmod("serverRegisterPipe", 0660);

        printf("Opening pipe in read only mode\n");
        
        int itineraryRequestPipe_fd = open("serverRegisterPipe", O_RDONLY);

        printf("Accepting T%d itinerary\n", i+1);

        int stagesNumber = 0;
        while(receiveStage(itineraryRequestPipe_fd,receivedStage)) {
            printf("Server received stage %s from register\n", receivedStage);
            
            strcpy(train[i].itinerary[stagesNumber], receivedStage);
            
            printf("Stage %s saved into T%d itinerary\n\n", receivedStage, i+1);
            stagesNumber++;
        }

        // for (int j = 0; j < SIZEOF(train[i].itinerary); ++j)printf("%s\n", train[i].itinerary[j]);

        printf("T%d route recieved\n", i+1);

        unlink("serverRegisterPipe");
    }

    printf("Itineraries for every train has been received... preparing to communicate with trains\n");
    sleep(2);

    /* Now server can start to wait for trains requests, it will setup a socket for this kind of communication */
    printf("Setting up socket...\n");
    
    int serverFd, clientFd, serverLen, clientLen;
    struct sockaddr_un serverUNIXAddress; /*Server address */
    struct sockaddr* serverSockAddrPtr; /*Ptr to server address*/
    struct sockaddr_un clientUNIXAddress; /*Client address */
    struct sockaddr* clientSockAddrPtr;/*Ptr to client address*/

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
    
    /* Begin to accept client connections */
    while (1) {
        printf("Server RBC is waiting for requests from clients\n");
        
        // listen()
        listen (serverFd, 5); /* Maximum pending connection length */
        
        // accept()
        clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
        
        /* First receive the number of the train that is making the request... */
        char trainNumber[2];
        readMessage(clientFd, trainNumber);
        /* ... and convert it to the int value that correspond to its position in train structure */
        int train_n = atoi(trainNumber) - 1;

        /* Then receive the segment or station that the train is requesting access to */
        char stage[5];
        readMessage(clientFd, stage);


        printf("T%s request access to %s\n", trainNumber, stage);

        /* If first char in stage is an S then train is requiring access to a station... */
        char isStation[2];
        sprintf(isStation, "%c", stage[0]);
        if (strcmp(isStation, "S") == 0) {
            /* ... so server has to return 2 and has to modify its data structures accordingly */
            char stationNumber[5];
            strncpy(stationNumber, stage+1, strlen(stage));
            // printf("stationNumber: %s\n", stationNumber);

            /* Station correspondent position in status.stations[] */
            int stationToIncrement = atoi(stationNumber) - 1;

            /* Update server data structure */
            // Increment value of the station occupated currently
            status.stations[stationToIncrement] += 1;

            // Decrement value of the track or station occupated previously
            char stageToDecrement[5];
                // retrieve which one through server data structures
            strncpy(stageToDecrement, train[train_n].itinerary[progresses[train_n]], 5); 
            printf("stage to decrement: %s\n", stageToDecrement);
            // increment progress of the train itinerary
            progresses[train_n] += 1;

            // Access to the stageToDecrement correspondent cell in status.segments or status.stations and decrement value
            char isStation[2];
            sprintf(isStation, "%c", stageToDecrement[0]);
            if (strcmp(isStation, "S") == 0) {
                char stationNumber[5];
                strncpy(stationNumber, stageToDecrement+1, strlen(stageToDecrement));
                printf("stationNumber: %s\n", stationNumber);
                int station_n = atoi(stationNumber) - 1;
                status.stations[station_n] -= 1;
            } else {
                char segmentNumber[5];
                strncpy(segmentNumber, stageToDecrement+2, strlen(stageToDecrement));
                printf("segmentNumber: %s\n", segmentNumber);
                int segment_n = atoi(segmentNumber) - 1;
                status.segments[segment_n] -= 1;
            }
            /* say "2" to train, that means that it has entered in a station */
            write(clientFd, "2", strlen("2") + 1);
        
        /* If first char in stage is not "S" then traini is requiring access to a track segment... */
        } else {
            /* ... so server has to return 1 or 0 if access is authorized or not authorized */
            char segmentNumber[5];
            strncpy(segmentNumber, stage+2, strlen(stage));
            printf("segmentNumber: %s\n", segmentNumber);

             /* Segment correspondent position in status.segments[] */
            int segmentToCheck = atoi(segmentNumber) - 1;

            // printf("Accessing to %s, finding %d\n", stage, status.segments[segmentToCheck]);

            /* If segment status = 0, segment is free and server give authorization to move and update data structures accordingly */
            if (status.segments[segmentToCheck] == 0) {
                
                /* Update server data structure */
                // Increment value of the station occupated currently
                status.segments[segmentToCheck] = 1;

                // Decrement value of the track or station occupated previously
                char stageToDecrement[5];
                    // retrieve which one through server data structures
                strncpy(stageToDecrement, train[train_n].itinerary[progresses[train_n]], 5); 
                printf("stage to decrement: %s\n", stageToDecrement);
                // increment progress of the train itinerary
                progresses[train_n] += 1;

                // Access to the stageToDecrement correspondent cell in status.segments or status.stations and decrement value
                char isStation[2];
                sprintf(isStation, "%c", stageToDecrement[0]);
                if (strcmp(isStation, "S") == 0) {
                    char stationNumber[5];
                    strncpy(stationNumber, stageToDecrement+1, strlen(stageToDecrement));
                    printf("stationNumber: %s\n", stationNumber);
                    int station_n = atoi(stationNumber) - 1;
                    status.stations[station_n] -= 1;
                } else {
                    char segmentNumber[5];
                    strncpy(segmentNumber, stageToDecrement+2, strlen(stageToDecrement));
                    printf("segmentNumber: %s\n", segmentNumber);
                    int segment_n = atoi(segmentNumber) - 1;
                    status.segments[segment_n] -= 1;
                }
                /* say "1" to train, that means that it is authorized to enter in the requested segment */
                write(clientFd, "1", strlen("1") + 1);

             /* If segment status = 1, segment is occupied and server doesn't give authorization to move.
                No updates to data structures needed */
            } else if(status.segments[segmentToCheck] == 1) {
                write(clientFd, "0", strlen("0") + 1);
            }   
        }
        /* Close socket when receive termination signal */  
        close(clientFd);
    }
    return 0;
}

/////////// SANTO SISTEMA, sono praticamente lo stesso metodo,
// cambia comportamento ricevendo in input un fd socket o pipe
// sia qui che in altri file viene usato, lo possiamo spostare in header per ridurre dup codice
int receiveStage(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(((n > 0) && (*str++ != '\0')));
    return (n > 0);
}

int readMessage (int fd, char *str) {
    /* Read a single ’\0’-terminated line into str from fd */
    int n;
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
    } while (n > 0 && *str++ != '\0');
    return (n > 0); 
}

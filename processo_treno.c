#include "header.h"

void startingMissionSignalHandler(int signum) {
    //do nothing, just awake the train from SIGSTOP
}

int requestAccessTo(char step[5], const char* MODE, int clientFd, const char* trainNumber);

int main(int argc, char const *argv[]) {
    
    /* Install signal SIGCONT to synchronize trains in starting their mission */
    signal(SIGCONT, startingMissionSignalHandler);

    const char * ETCS = argv[2];
    const char* trainNumber = argv[1];

    int clientFd, serverLen, result;
    struct sockaddr_un serverUNIXAddress;
    struct sockaddr* serverSockAddrPtr;
    /* Prepare socket for communication with server if ETCS2 */
    if (strcmp(ETCS, "ETCS2") == 0) {
        // socket()
        serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
        serverLen = sizeof (serverUNIXAddress);
        //clientFd = socket (AF_UNIX, SOCK_STREAM, 0);
        serverUNIXAddress.sun_family = AF_UNIX; /* Server domain */
        strcpy (serverUNIXAddress.sun_path, "authorization"); /*Server name*/
    } else {
        clientFd = 0;
    }

    /* Preparing request message for process registro to ask for the itinerary */
    int itineraryRequestPipe_fd, requestLen; 
    char request[5];
	sprintf(request, "T%s", trainNumber);
    requestLen = strlen (request) + 1;

    /* Use the designated pipe created by process registro to make request */
    char requestPipeName[30];
    sprintf(requestPipeName, "T%sitineraryRequestPipe", trainNumber);

    do { /* try open pipe until successful */
        itineraryRequestPipe_fd = open (requestPipeName, O_WRONLY);
        /* 
        DEBUG: check open result  to see when registro process will create the pipe
        printf("%s prova ad aprire %s, risultato: %d\n", request, requestPipeName, itineraryRequestPipe_fd);
        */
        if (itineraryRequestPipe_fd == -1) sleep (1); /* Try again after one second if fail */
    } while (itineraryRequestPipe_fd == -1);

    /* Send request through pipe, then close and delete it */
    write(itineraryRequestPipe_fd, request, requestLen);
    close(itineraryRequestPipe_fd);
    unlink(requestPipeName);

    /* Preparing to receive itinerary */
    int registerToTrainPipe_fd;
    char receivedStage[5];
    char itinerary[10][5];
    int stagesNumber = 0;
    char itineraryPipeName[30];
    sprintf(itineraryPipeName, "T%sregisterPipe", trainNumber);
    registerToTrainPipe_fd = open(itineraryPipeName, O_RDONLY);

    /* Receive stages until pipe is closed on writer side */
    while(receiveFrom(registerToTrainPipe_fd, receivedStage)) {
        // Save stage in own data structure
        // printf("Saving %s into T%s\n", receivedStage, trainNumber);
        strcpy(itinerary[stagesNumber], receivedStage);
        stagesNumber++;
    }
    
    close(registerToTrainPipe_fd);
    unlink(itineraryPipeName);

    printf("T%s: itinerary received from registro\n", trainNumber);
    
    /* wait for a SIGCONT from register 
        to start together with all the other trains at the same time */
    kill(getpid(), SIGSTOP);

    printf("T%s:", trainNumber);
    for (int j = 0; j < stagesNumber; ++j) printf("->%s", itinerary[j]);
    printf("\n");

    /* TRAIN MISSION */

    /* Preparing to start mission */
    // Create logFile
    FILE * logFile;
    char logFileName[10];
    sprintf(logFileName, "../log/T%s.log",trainNumber);
    logFile = fopen(logFileName, "w");
    // Utilities for writing date and time in logfile
    time_t rawtime;
    struct tm * timeinfo;
    char dateAndTime [30];

    // Preparing variables for turnation to access track files
    FILE * trackFilesGuard;
    char trackFilesGuardName[20];
    sprintf(trackFilesGuardName, "trackFilesGuard");

    // Preparing variables for execution
    FILE * trackFile;
    char * currentStage;
    char * nextStage;
    int j = 0;
    
    while(j < stagesNumber) {
        /* Train wait for its turn to access track files,
            turnation is managed by process turn_manager,
            turn_manager change turn writing in the file trackFilesGuard,
            turn change every second in this order 1-2-3-4-[5]-1-2-3...*/
        char turn[2];
        do { /* Repeat until it's train turn */
            trackFilesGuard = fopen(trackFilesGuardName, "r");
            fseek(trackFilesGuard, 0, SEEK_SET);
            int r;
            r = fread(turn, 1, 1, trackFilesGuard);
            /* DEBUG: checking that the correct train got access according to turn
                printf("T%s check that it's T%s turn\n", trainNumber, turn); 
                */
            fclose(trackFilesGuard);
            sleep(1);
        } while(strcmp(turn, trainNumber) != 0);

        // A. Read next track segment or station
        currentStage = itinerary[j];
        nextStage = itinerary[j+1];

        /* log file update 
            preparing string to write */
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (dateAndTime, 30, "%d %B %Y %X",timeinfo);
        char logUpdate[100];
        sprintf(logUpdate, "[Attuale: %s], [Next: %s], %s\n", currentStage, nextStage, dateAndTime);
        // write update in logfile
        fwrite(logUpdate, sizeof(char), strlen(logUpdate), logFile);

        // B. Request authorization to access the segment or station
        clientFd = socket (AF_UNIX, SOCK_STREAM, 0);
        //first connect to server
        if(strcmp(ETCS, "ETCS2") == 0) {
            do { /* Loop until a connection is made with the server */
                result = connect (clientFd, serverSockAddrPtr, serverLen);
                if (result == -1){
                    printf("T%s: server not ready!\n", trainNumber);
                    sleep (1); /* Wait and then try again */
                }
            } while (result == -1);
            printf("T%s: server ready\n", trainNumber);
        }

        int authorization = requestAccessTo(nextStage, ETCS, clientFd, trainNumber);
        
        if (authorization == 2) { /* Train got authorization to access a station */
            printf("T%s: Access to %s authorized\n", trainNumber, nextStage);  
            /* Set to 0 the content of the file of the track occupated previously */
            trackFile = fopen(currentStage, "w");
            fseek(trackFile, 0, SEEK_SET);
            fwrite("0", sizeof(char), 1, trackFile);
            fclose(trackFile);
            /* Terminate mission */
            break;

        } else if(authorization == 1) { /* Train got authorization to access a track segment */
            printf("T%s: Access to %s authorized\n", trainNumber, nextStage);
            /* Set to 1 the content of the file representing the track occupated now */
            trackFile = fopen(nextStage, "w");
            fseek(trackFile, 0, SEEK_SET);
            fwrite("1", sizeof(char), 1, trackFile);
            fclose(trackFile);
            /* Set to 0 the content of the file of the track occupated previously
                checking if previous stage is a station, otherwise it will create the file Sx*/
            char isStation[2];
            sprintf(isStation, "%c", currentStage[0]);
            if (strcmp(isStation, "S") != 0) {
                trackFile = fopen(currentStage, "w");
                fseek(trackFile, 0, SEEK_SET);
                fwrite("0", sizeof(char), 1, trackFile);
                fclose(trackFile);
            }
            j++;

        } else if (authorization == 0) { /* Train is not authorized to proceed */
            printf("T%s: Access to %s not authorized\n", trainNumber, nextStage);
        } else {
            perror("authorization error\n");
            exit(EXIT_FAILURE);
        }
        /* Closesocket to let other trains connect */
        close (clientFd);
        // C. sleep 2 seconds
        sleep(2);
        // D. repeat from A
    }
    
    /* Last update to logFile before terminating */
    strftime (dateAndTime, 30, "%d %B %Y %X",timeinfo);
    char logUpdate[100];
    sprintf(logUpdate, "[Attuale: %s], [Next: --], %s\n", nextStage, dateAndTime);
    // write update in logfile
    fwrite(logUpdate, sizeof(char), strlen(logUpdate), logFile);

    fclose(logFile);
    

    printf("T%s: ### Train T%s reach station %s. Mission complete! ###\n", trainNumber, trainNumber, nextStage);
    
    /* Send a SIGUSR1 to parent pid before terminating */ 
    kill(getppid(), SIGUSR1);

    // printf("T%s: process terminated...\n", trainNumber);

    while(1){
        printf("T%s: waiting for other trains to terminate their missions...\n", trainNumber);
        sleep(5);
    }

    return 0;
}


/* REQUEST ACCESS TO */
/* This function select the method to request authorization to proceed on a 
    track according to ETCS:
    return 2 if access to a station is authorized
    return 1 if access to a track is authorized 
    return 0 if access to next stage is not authorized */    
int requestAccessTo(char stage[5], const char* ETCS, int clientFd, const char* trainNumber){
   
    printf("T%s: request access to %s\n", trainNumber, stage);

    FILE * stageFile;

    /* If mode ETCS1 access is authorized by reading the file content: 
        if 0 access is authorized
        if 1 access is not authorized
        access t stations is always authorized */
    if(strcmp(ETCS, "ETCS1") == 0) {
        /* If next stage is a train station access is automatically guaranteed */
        char isStation[2];
        sprintf(isStation, "%c", stage[0]);
        if (strcmp(isStation, "S") == 0) 
            return 2;
        
        /* Otherwise open file correspondent to the stage it want to acceed */
        stageFile = fopen(stage, "r");
        /* Read character contained in next stage file */
        char fileContent[2];
        int i;
        i = fread(fileContent, 1, 1, stageFile);
        fclose(stageFile);
        
        /* DEBUG: to check requested stage status
        printf("%s statust: %s\n", stage, fileContent);
        */
        if (fileContent[0]=='0') 
            return 1;
        else if (fileContent[0]=='1') 
            return 0;
        else
            perror("Something went wrong in reading file\n");
    /* If mode ETCS2 access is authorized asking to server_RBC and reading its answer:
        if server_RBC replies with 0 access is not authorized
        if server_RBC replies with 1 access to a track is authorized
        if server_RBC replies with 2 access to a station is authorized */
    } else if (strcmp(ETCS, "ETCS2") == 0) {
        /* Request to server the authorization to access stage */
            
        // First send train number...
        write(clientFd, trainNumber, strlen(trainNumber) + 1);
        // ... then send requested stage
        write(clientFd, stage, strlen(stage) + 1);

        // Read answer from server, that will be 0, 1 or 2
        char str[3];
        receiveFrom(clientFd, str);
        /* DEBUG: check correct value is received from server
        printf("%s\n", str);
        */

        // convert answer to int and return it
        int authorization = atoi(str);
        return authorization;
    } else {
        perror("MODE not recognized\n");
        exit(EXIT_SUCCESS);
    }

}




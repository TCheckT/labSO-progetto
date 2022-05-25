#include "header.h"

void signalHandler(int signum);

int main(int argc, char const *argv[]) {
    
    const char * ETCS = argv[2];
    const char* trainNumber = argv[1];

    /* Install signal to synchronize trains in starting their mission */
    signal(SIGCONT, signalHandler);

    /* Preparing request message for process registro to ask for the itinerary */
    int itineraryRequestPipe_fd, requestLen; 
    char request [5];
	sprintf(request, "T%s", trainNumber);
    requestLen = strlen (request) + 1;

    /* Use the designated pipe created by process registro */
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
    while(receiveStage(registerToTrainPipe_fd, receivedStage)) {
        // Save stage in own data structure
        // printf("Saving %s into T%s\n", receivedStage, trainNumber);
        strcpy(itinerary[stagesNumber], receivedStage);
        stagesNumber++;
    }
    close(registerToTrainPipe_fd);
    unlink(itineraryPipeName);

    printf("T%s received its itinerary\n", trainNumber);
    printf("T%s:", trainNumber);
    for (int j = 0; j < stagesNumber; ++j) printf("->%s", itinerary[j]);
    printf("\n");

    /* wait for a SIGCONT from register 
        to start together with all the other trains at the same time */
    kill(getpid(), SIGSTOP);

    /* 
    DEBUG: check if all trains start their mission when register dies 
    printf("Does T%s restart?\n", trainNumber);
    */

    /* TRAIN MISSION */
    int trainID = atoi(trainNumber);

    /* Preparing to start mission */
    // Create logFile
    FILE * logFile;
    char logFileName[10];
    sprintf(logFileName, "T%s.log",trainNumber);
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
        /* Wait for my turn to access track files,
            turnation is managed by process turn_manager,
            turn_manager change turn writing in the file trackFilesGuard,
            turn change every second in this order 1-2-3-4-[5]-1-2-3...*/
        char turn[2];
        do { /* Repeat until it's my turn */
            trackFilesGuard = fopen(trackFilesGuardName, "r");
            fseek(trackFilesGuard, 0, SEEK_SET);
            int r;
            r = fread(turn, 1, 1, trackFilesGuard);
            printf("T%s check that it's T%s turn\n", trainNumber, turn);
            fclose(trackFilesGuard);
            sleep(1);
        } while(strcmp(turn, trainNumber) != 0);

        printf("T%s got access to files\n", trainNumber);

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
        int authorization = requestAccessTo(nextStage, ETCS);
        
        if (authorization == 2) { /* If nextStage is a station */
            printf("T%s: Access to %s authorized\n", trainNumber, nextStage);  
            // Set to 0 the content of the file of the track occupated previously
            /* check if previous stage is a station, otherwise it will create the file
                THIS CHECK CAN BE REMOVED, not possible to have a station before a station */
            char isStation[2];
            sprintf(isStation, "%c", currentStage[0]);
            if (strcmp(isStation, "S") != 0) {
                trackFile = fopen(currentStage, "w");
                fseek(trackFile, 0, SEEK_SET);
                fwrite("0", sizeof(char), 1, trackFile);
                fclose(trackFile);
            }
            // then terminate mission
            break;

        } else if(authorization == 1) { /* If nextStage is a track */
            printf("T%s: Access to %s authorized\n", trainNumber, nextStage);
            // Set to 1 the content of the file of the track occupated now
            trackFile = fopen(nextStage, "w");
            fseek(trackFile, 0, SEEK_SET);
            fwrite("1", sizeof(char), 1, trackFile);
            fclose(trackFile);
            // Set to 0 the content of the file of the track occupated previously
            /* check if previous stage is a station, otherwise it will create the file */
            char isStation[2];
            sprintf(isStation, "%c", currentStage[0]);
            if (strcmp(isStation, "S") != 0) {
                trackFile = fopen(currentStage, "w");
                fseek(trackFile, 0, SEEK_SET);
                fwrite("0", sizeof(char), 1, trackFile);
                fclose(trackFile);
            }
            j++;

        } else if (authorization == 0) { /* If access not authorized */
            printf("T%s: Access to %s not authorized\n", trainNumber, nextStage);
        } else {
            perror("authorization error\n");
            exit(EXIT_FAILURE);
        }

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

    printf("Treno T%s terminate its mission!!! CONGRATULAZIONI! clap, clap, clap...\n", trainNumber);

    exit(EXIT_SUCCESS);
    return 0;
}

/* This function read a /0 terminating sequence of char from a pipe fd 
    and place them as a string at str */
int receiveStage(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}

/* This function select the method to request authorization to proceed on a 
    track according to ETCS:
    return 2 if access to a station is authorized
    return 1 if access to a track is authorized 
    return 0 if access to next stage is not authorized */    
int requestAccessTo(char stage[5], const char* ETCS){
   
    printf("Requesting access for %s\n", stage);

    FILE * stageFile;

    if(strcmp(ETCS, "ETCS1") == 0) {
        /* If next stage is a train station access is automatically guaranteed */
        char isStation[2];
        sprintf(isStation, "%c", stage[0]);
        if (strcmp(isStation, "S") == 0) 
            return 2;
        
        stageFile = fopen(stage, "r");
        
        /* Read character contained in next stage file */
        char fileContent[2];
        int i;
        i = fread(fileContent, 1, 1, stageFile);
        fclose(stageFile);
        
        /* DEBUG: to check requested stage status
        printf("%s statust: %s\n", stage, fileContent);
        */
        if (strcmp(fileContent, "0") == 0) 
            return 1;

        return 0;

    } else if (strcmp(ETCS, "ETCS2") == 0) {
        // requesting to server
        printf("missing server request implementation\n");
        return -1;
    } else {
        perror("MODE not recognized\n");
        exit(EXIT_SUCCESS);
    }

}

void signalHandler(int signum){
    //do nothing just restart
}
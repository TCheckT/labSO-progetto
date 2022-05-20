#include "header.h"

void signalHandler(int signum);

int main(int argc, char const *argv[]) {
    
    const char* trainNumber = argv[1];

    /* install signal to synchronize trains in starting their mission */
    signal(SIGCONT, signalHandler);

    // preparing request
     int itineraryRequestPipe_fd, requestLen; char request [5];
	sprintf(request, "T%s", trainNumber);
    requestLen = strlen (request) + 1;

    char requestPipeName[30];
    sprintf(requestPipeName, "T%sitineraryRequestPipe", trainNumber);
    do { // try open pipe until successful 
        itineraryRequestPipe_fd = open (requestPipeName, O_WRONLY);
        /* DEBUG: check open result */
        // printf("%s prova ad aprire %s, risultato: %d\n", request, requestPipeName, itineraryRequestPipe_fd);
        if (itineraryRequestPipe_fd == -1) sleep (1); // Try again after one second if fail
    } while (itineraryRequestPipe_fd == -1);

    // send request
    write (itineraryRequestPipe_fd, request, requestLen);

    close(itineraryRequestPipe_fd);
    unlink(requestPipeName);

    // Preparing to receive itinerary
    int registerToTrainPipe_fd;
    char receivedStage[5];
    char itinerary[10][5];
    int i = 0;
    char itineraryPipeName[30];
    sprintf(itineraryPipeName, "T%sregisterPipe", trainNumber);
    registerToTrainPipe_fd = open(itineraryPipeName, O_RDONLY);

    while(receiveStage(registerToTrainPipe_fd, receivedStage)) {
        //save stage in own data structure
        printf("Saving %s into T%s\n", receivedStage, trainNumber);
        strcpy(itinerary[i], receivedStage);
        i++;
    }
    printf("T%s:", argv[1]);
    for (int j = 0; j < i; ++j) printf("->%s", itinerary[j]);
    printf("\n");
    close(registerToTrainPipe_fd);
    unlink(itineraryPipeName);

    // waiting for a SIGCONT from register to start with all the other trains
    kill(getpid(), SIGSTOP);

    printf("treno T%s ready\n", argv[1]);
  
    // TODO: train behaviour, this is a placeholder
    int count = 0;
    while(count < 2){
        count++;
        printf("##########T%s is doing stuff number %d with mode %s... \n", trainNumber, count, argv[2]);
        sleep(1);    
    }
   
    exit(EXIT_SUCCESS);
    return 0;
}

int receiveStage(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}

void signalHandler(int signum){
    //do nothing just restart
}
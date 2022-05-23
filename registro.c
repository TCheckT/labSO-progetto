#include "header.h"

int sendStation(char* itinerary, char * pipe, int itineraryLen);

int main(int argc, char *argv[]) {

    char* MAPPA = argv[1];
    char* ETCS = argv[2];
    // printf("%s received from movementAuthority\n", ETCS);

    // setting train number according to MAPPA
    const int numberOfTrains=(strcmp(argv[1], "MAPPA1") == 0) ? 4 : 5;
    
    char * M1itineraryT1[] = {"S1", "MA1", "MA2", "MA3", "MA8", "S6"};
    char * M1itineraryT2[] = {"S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6"};
    char * M1itineraryT3[] = {"S7", "MA13", "MA12", "MA11", "MA10", "MA9", "S3"};
    char * M1itineraryT4[] = {"S4", "MA14", "MA15", "MA16", "MA12", "S8"};
    char * M1itineraryT5[] = {NULL};


    char * M2itineraryT1[] = {"S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6"};
    char * M2itineraryT2[] = {"S3", "MA9", "MA10", "MA11", "MA12", "S8"};
    char * M2itineraryT3[] = {"S4", "MA14", "MA15", "MA16", "MA12", "S8"};
    char * M2itineraryT4[] = {"S6", "MA8", "MA3", "MA2", "MA1", "S1"};
    char * M2itineraryT5[] = {"S5", "MA4", "MA3", "MA2", "MA1", "S1"};
    
    int itineraryRequestPipe_fd;
    char mapRequest[7];
    
    //TODO: sending all itineraries to serverRBC
    if(strcmp(ETCS, "ETCS2") == 0) {
        // opening pipe with server in read-only mode
        itineraryRequestPipe_fd = open("serverRegisterPipe",O_RDONLY);
        // getting the server map request
        waitForRequest(itineraryRequestPipe_fd, mapRequest);
        // closing pipe
        unlink("serverRegisterPipe");

        itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
        mknod("serverRegisterPipe", S_IFIFO, 0);
        chmod("serverRegisterPipe", 0660);

        if (strcmp(mapRequest,"MAPPA1")==0)
        {
            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M1itineraryT1[0], "serverRegisterPipe", SIZEOF(M1itineraryT1[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M1itineraryT2[0], "serverRegisterPipe", SIZEOF(M1itineraryT2[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M1itineraryT3[0], "serverRegisterPipe", SIZEOF(M1itineraryT3[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M1itineraryT4[0], "serverRegisterPipe", SIZEOF(M1itineraryT4[0]));
        }else {
            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M2itineraryT1[0], "serverRegisterPipe", SIZEOF(M2itineraryT1[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M2itineraryT2[0], "serverRegisterPipe", SIZEOF(M2itineraryT2[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M2itineraryT3[0], "serverRegisterPipe", SIZEOF(M2itineraryT3[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M2itineraryT4[0], "serverRegisterPipe", SIZEOF(M2itineraryT4[0]));

            sleep(1);
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendStation(M2itineraryT5[0], "serverRegisterPipe", SIZEOF(M2itineraryT5[0]));
        }
    }

    printf("regiser sent stations to server\n");
    
    // preparing to receive requests from trains
    char receivedRequest[5];
    int satisfiedRequests = 0;
    char requestPipeName[30];

    while(satisfiedRequests < numberOfTrains) {
        printf("Register creating pipe to send itinerary to trains\n");
        //Create and open pipe for receiving itinerary from a train
        sprintf(requestPipeName, "T%ditineraryRequestPipe", satisfiedRequests + 1);
        unlink(requestPipeName);
        mknod(requestPipeName, S_IFIFO, 0);
        chmod(requestPipeName, 0660);

        itineraryRequestPipe_fd = open(requestPipeName, O_RDONLY);
        // printf("waiting for requests...\n");

        waitForRequest(itineraryRequestPipe_fd, receivedRequest);
        printf("Request for itinerary from %s received\n", receivedRequest);

        // preparing to send itinerary
        int i;
        i = assignItinerary(receivedRequest);
        
        // printf("sto per inviare a T%d\n", i);

        // send itinerary to train selecting it with MAPPA and i
        if(strcmp(MAPPA, "MAPPA1") == 0) {     
            if(i==1)sendItinerary(M1itineraryT1, 1, SIZEOF(M1itineraryT1));
            else if(i==2) sendItinerary(M1itineraryT2, 2, SIZEOF(M1itineraryT2));
            else if(i==3) sendItinerary(M1itineraryT3, 3, SIZEOF(M1itineraryT3));
            else if(i==4) sendItinerary(M1itineraryT4, 4, SIZEOF(M1itineraryT4));
            else if(i==5) sendItinerary(M1itineraryT5, 5, SIZEOF(M1itineraryT5));
            else perror("Error: unexpected request");
        } else if (strcmp(MAPPA, "MAPPA2") == 0) {
            if(i==1) sendItinerary(M2itineraryT1, 1, SIZEOF(M2itineraryT1));
            else if(i==2) sendItinerary(M2itineraryT2, 2, SIZEOF(M2itineraryT2));
            else if(i==3) sendItinerary(M2itineraryT3, 3, SIZEOF(M2itineraryT3));
            else if(i==4) sendItinerary(M2itineraryT4, 4, SIZEOF(M2itineraryT4));
            else if(i==5) sendItinerary(M2itineraryT5, 5, SIZEOF(M2itineraryT5));
            else perror("Error: unexpected request");
        } else perror("Error: MAPPA not recognized\n");
        
        satisfiedRequests++;

        // printf("%d requests have been satisfied\n", satisfiedRequests);

        // close and remove pipe used
        close(itineraryRequestPipe_fd);
        unlink(requestPipeName);
    }

    sleep(1); /* wait a while that all trains collect their stages */
    
    // SIGCONT to stopped trains before terminating
    printf("Register dying sending SIGCONT to processes in group %d\n", getpgid(getpid()));
    kill(0, SIGCONT);

    exit(EXIT_SUCCESS);
    return 0;
}

// function for reading from a pipe fd and compose string str
int waitForRequest(int fd, char *str) { 
    int n;
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
    } while (n > 0 && *str++ != 0);
    return (n > 0); /* Return false if end-of-input */
}

int sendItinerary(char* itinerary[], int r, int itineraryLen) {
    
    // preparing to send into pipe
    int sendingToTrain_fd, stageLen, i;
    char itineraryPipeName[30];
    sprintf(itineraryPipeName,"T%dregisterPipe", r);

    // printf("Using %s\n", itineraryPipeName);
    
    do { // try open pipe until successful 
        sendingToTrain_fd = open (itineraryPipeName, O_WRONLY);
        if (sendingToTrain_fd == -1) sleep (1); // Try again after one second if fail
    } while (sendingToTrain_fd == -1);

    // Send all stages into pipe
    for (i = 0; i < itineraryLen; i++) { 
        char* tappa = itinerary[i];
        stageLen = strlen(tappa) + 1;
        printf("Sending %s through pipe to train\n", tappa);
        write(sendingToTrain_fd, tappa, stageLen);   
    }
    close(sendingToTrain_fd);
    unlink(itineraryPipeName);
    return 0;
}

int sendStation(char* itinerary, char * pipe, int itineraryLen) {
    
    // preparing to send into pipe
    int sendingToServer_fd, stageLen, i;

    do { // try open pipe until successful 
        sendingToServer_fd = open ( pipe, O_WRONLY);
        if (sendingToServer_fd == -1) sleep (1); // Try again after one second if fail
    } while (sendingToServer_fd == -1);

    stageLen = strlen(itinerary) + 1;
    printf("Sending %s through pipe to server\n", itinerary);
    write(sendingToServer_fd, itinerary, stageLen);   

    close(sendingToServer_fd);
    //unlink(pipe);
    return 0;
}

// function to assign which itinerary will be sent from register to train
int assignItinerary(char * request){
    if(strcmp(request, "T1") == 0)          return 1;
    else if (strcmp(request, "T2") == 0)    return 2;
    else if (strcmp(request, "T3") == 0)    return 3;
    else if (strcmp(request, "T4") == 0)    return 4;
    else if (strcmp(request, "T5") == 0)    return 5;
    else                                    return -1;  
}
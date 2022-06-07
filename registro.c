#include "header.h"

/*  */
int sendItinerary(char* itinerario[], char numeroTreno[], int lunghezzaItinerario);
/*  */
int assignItinerary(char[]);

int main(int argc, char *argv[]) {
    /* registro contains all itineraries for both MAPPAs and send them
        to processes that make a request through pipes */

    char* MAPPA = argv[1];
    char* ETCS = argv[2];

    /* Setting train number according to MAPPA */
    const int numberOfTrains=(strcmp(argv[1], "MAPPA1") == 0) ? 4 : 5;
    
    /* Hardcoding itineraries */
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
    
    /* If ETCS2 mode, server_RBC need all itineraries of the MAPPA
        before starting to accept trains requests.
        registro send all itineraries to serverRBC */
    if(strcmp(ETCS, "ETCS2") == 0) {
        if (strcmp(MAPPA,"MAPPA1")==0) {
            sleep(1);
            printf("registro: sending itinerary T1 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M1itineraryT1, "serverRegisterPipe", SIZEOF(M1itineraryT1));
            close(itineraryRequestPipe_fd);
            
            sleep(1);
            printf("registro: sending itinerary T2 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M1itineraryT2, "serverRegisterPipe", SIZEOF(M1itineraryT2));
            close(itineraryRequestPipe_fd);

            sleep(1);
            printf("registro: sending itinerary T3 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M1itineraryT3, "serverRegisterPipe", SIZEOF(M1itineraryT3));
            close(itineraryRequestPipe_fd);

            sleep(1);
            printf("registro: sending itinerary T4 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M1itineraryT4, "serverRegisterPipe", SIZEOF(M1itineraryT4));
            close(itineraryRequestPipe_fd);
        } else {
            sleep(1);
            printf("registro: sending itinerary T1 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M2itineraryT1, "serverRegisterPipe", SIZEOF(M2itineraryT1));
            close(itineraryRequestPipe_fd);
            
            sleep(1);
            printf("registro: sending itinerary T2 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M2itineraryT2, "serverRegisterPipe", SIZEOF(M2itineraryT2));
            close(itineraryRequestPipe_fd);

            sleep(1);
            printf("registro: sending itinerary T3 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M2itineraryT3, "serverRegisterPipe", SIZEOF(M2itineraryT3));
            close(itineraryRequestPipe_fd);

            sleep(1);
            printf("registro: sending itinerary T4 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M2itineraryT4, "serverRegisterPipe", SIZEOF(M2itineraryT4));
            close(itineraryRequestPipe_fd);

            sleep(1);
            printf("registro: sending itinerary T5 to server\n");
            itineraryRequestPipe_fd = open("serverRegisterPipe",O_WRONLY);
            sendItinerary(M2itineraryT5, "serverRegisterPipe", SIZEOF(M2itineraryT5));
            close(itineraryRequestPipe_fd);
        } 
        printf("registro: all itineraries have been sent to server_RBC\n");
    }

    
    /* Preparing to receive requests from trains */
    char receivedRequest[5];
    int satisfiedRequests = 0;
    char requestPipeName[30];

    /* Wait for request from a train and assign itinerary to it when received,
        repeat it until every train send a request and receive its itinerary */
    while(satisfiedRequests < numberOfTrains) {
        // Create and open pipe for receiving itinerary from a train
        sprintf(requestPipeName, "T%ditineraryRequestPipe", satisfiedRequests + 1);
        unlink(requestPipeName);
        mknod(requestPipeName, S_IFIFO, 0);
        chmod(requestPipeName, 0660);

        itineraryRequestPipe_fd = open(requestPipeName, O_RDONLY);

        receiveFrom(itineraryRequestPipe_fd, receivedRequest);
        printf("registro: request from %s received\n", receivedRequest);

        // Preparing to send the correct itinerary according to the request received 
        int selectedTrain = assignItinerary(receivedRequest);

        // Send itinerary to train selecting it with MAPPA and selectedTrain
        
        if(strcmp(MAPPA, "MAPPA1") == 0) {     
            switch(selectedTrain){
                case 1: 
                    sendItinerary(M1itineraryT1, "T1registerPipe", SIZEOF(M1itineraryT1));
                    break;
                case 2: 
                    sendItinerary(M1itineraryT2, "T2registerPipe", SIZEOF(M1itineraryT2));
                    break;
                case 3: 
                    sendItinerary(M1itineraryT3, "T3registerPipe", SIZEOF(M1itineraryT3));
                    break;
                case 4: 
                    sendItinerary(M1itineraryT4, "T4registerPipe", SIZEOF(M1itineraryT4));
                    break;
                case 5: 
                    sendItinerary(M1itineraryT5, "T5registerPipe", SIZEOF(M1itineraryT5));
                    break;
                default:
                    perror("Error: unexpected request");
            }
        } else if (strcmp(MAPPA, "MAPPA2") == 0) {
            switch(selectedTrain){
                case 1: 
                    sendItinerary(M2itineraryT1, "T1registerPipe", SIZEOF(M2itineraryT1));
                    break;
                case 2: 
                    sendItinerary(M2itineraryT2, "T2registerPipe", SIZEOF(M2itineraryT2));
                    break;
                case 3: 
                    sendItinerary(M2itineraryT3, "T3registerPipe", SIZEOF(M2itineraryT3));
                    break;
                case 4: 
                    sendItinerary(M2itineraryT4, "T4registerPipe", SIZEOF(M2itineraryT4));
                    break;
                case 5: 
                    sendItinerary(M2itineraryT5, "T5registerPipe", SIZEOF(M2itineraryT5));
                    break;
                default:
                    perror("Error: unexpected request");
            }
        } else perror("Error: MAPPA not recognized\n");
        
        satisfiedRequests++;
        printf("registro: itinerary for T%d has been sent\n", selectedTrain);
       
        // Close and remove pipe used
        close(itineraryRequestPipe_fd);
        unlink(requestPipeName);
    }
    sleep(1); /* wait a while that all trains collect their stages */
    
    printf("registro: All itineraries have been sent, trains can start their mission...\n");
    /* Send a SIGCONT to stopped trains before terminating, 
        when a train received its itinerary it stop itself with a SIGSTOP,
        now they can resume execution */
    kill(0, SIGCONT);
    
    printf("registro: process terminated...\n");

    exit(EXIT_SUCCESS);
    return 0;
}

/* SEND ITINERARY */
/* This function send, one by one, the stages of an itineraries 
    to the respective train, using the appropriate pipe */
int sendItinerary(char* itinerary[], char pipeName[], int itineraryLen) {
    
    // Preparing to send into pipe
    int fd, stageLen, i;
    
    do { /* try open pipe until successful */
        fd = open (pipeName, O_WRONLY);
        if (fd == -1) sleep (1); /* Try again after one second if fails */
    } while (fd == -1);

    // Send all stages into pipe
    for (i = 0; i < itineraryLen; i++) { 
        char* stage = itinerary[i];
        stageLen = strlen(stage) + 1;
        // printf("Sending %s through pipe to train\n", stage);
        sleep(0.5);
        write(fd, stage, stageLen);   
    }
    close(fd);
    unlink(pipeName);
    return 0;
}

/* ASSIGN ITINERARY */
/* Function to assign which itinerary will be sent from register to train */
int assignItinerary(char * request){
    if(strcmp(request, "T1") == 0)          return 1;
    else if (strcmp(request, "T2") == 0)    return 2;
    else if (strcmp(request, "T3") == 0)    return 3;
    else if (strcmp(request, "T4") == 0)    return 4;
    else if (strcmp(request, "T5") == 0)    return 5;
    else                                    return -1;  
}

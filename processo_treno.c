#include "header.h"

void signalHandler(int signum);

int main(int argc, char const *argv[]) {
    
    const char * MODE = argv[2];

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
    printf("T%s:", trainNumber);
    for (int j = 0; j < i; ++j) printf("->%s", itinerary[j]);
    printf("\n");
    close(registerToTrainPipe_fd);
    unlink(itineraryPipeName);

    // waiting for a SIGCONT from register to start with all the other trains
    kill(getpid(), SIGSTOP);

    // Train behaviour
    char * currentStep;
    char * nextStep;
    int j = 0;
    FILE * file;

    while(j < i) {
        // A. Read next track segment or station
        currentStep = itinerary[j];
        nextStep = itinerary[j+1];

        // B. Request authorization to access the segment or station
        int authorization = requestAccessTo(nextStep, MODE);
        
        if (authorization == 2) { // if nextStep is a station
            printf("T%s: Access to %s authorized\n", trainNumber, nextStep);  
            // set to 0 occupation file of previous occupied track
            char isStation[2];
            sprintf(isStation, "%c", currentStep[0]);
            // check if prevois step is a station, otherwise it will create the file
            if (strcmp(isStation, "S") != 0) {
                file = fopen(currentStep, "w");
                fseek(file, 0, SEEK_SET);
                fwrite("0", sizeof(char), 1, file);
                fclose(file);
            }
            // then terminate mission
            break;

        } else if(authorization == 1) { // if nextStep is a segment
            printf("T%s: Access to %s authorized\n", trainNumber, nextStep);
            // set to 1 occupation file of current occupated track
            file = fopen(nextStep, "w");
            fseek(file, 0, SEEK_SET);
            fwrite("1", sizeof(char), 1, file);
            fclose(file);
            // set to 0 occupation file of previous occupied track
            char isStation[2];
            sprintf(isStation, "%c", currentStep[0]);
            // check if prevois step is a station, otherwise it will create the file
            if (strcmp(isStation, "S") != 0) {
                file = fopen(currentStep, "w");
                fseek(file, 0, SEEK_SET);
                fwrite("0", sizeof(char), 1, file);
                fclose(file);
            }
            j++;

        } else if (authorization == 0) { // if access not permitted
            printf("T%s: Access to %s not authorized\n", trainNumber, nextStep);
        } else {
            perror("authorization error\n");
            exit(EXIT_FAILURE);
        }
        // C. sleep 2 seconds
        sleep(2);
        // D. repeat from A
    }

    printf("Treno T%s terminate its mission!!! CONGRATULAZIONI! clap, clap, clap...\n", trainNumber);

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

    
int requestAccessTo(char step[5], const char* MODE){
   
    printf("Requesting access to %s\n", step);

    FILE * file;

    if(strcmp(MODE, "ECTS1") == 0) {
        // if next step is a train station access is automatically guaranteed
        char isStation[2];
        sprintf(isStation, "%c", step[0]);
        if (strcmp(isStation, "S") == 0) return 2;
        
        // opening file
        file = fopen(step, "r");

        //reading character
        char fileContent[2];
        int i;
        i = fread(fileContent, 1, 1, file);
        fclose(file);
        // printf("%s\n", fileContent);
        if (strcmp(fileContent, "0") == 0) 
            return 1;

        return 0;

    } else if (strcmp(MODE, "ECTS2") == 0) {
        // requesting to server
        printf("missing server request implementation\n");
        return -1;
    } else {
        perror("MODE not recognized\n");
        exit(EXIT_SUCCESS);
    }

}
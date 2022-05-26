#include "header.h"


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

/* ATTEMPT TO REALIZE OPTIONAL TASK 2: supposed to use SIGUSR1
void trainEndMissionHandler(int signum) {
    
    wait(NULL);
    //printf("Ricevo %d° SIGUSR1!\n", ++signalCounter);
    //sleep(1);
}
*/

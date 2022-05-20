#include "header.h"

void trainEndMissionHandler(int signum);

int signalCounter = 0;

int main(int argc, char *argv[]) {

    signal(SIGUSR1, trainEndMissionHandler);

    char* MAPPA = argv[1];
    char* MODE = argv[2];
    // PADRE_TRENI crea i segmenti di binario
    if(creaSegmenti()!=0)
        perror("errore creazione segmenti di binario\n");
    
    pid_t treni[5];


    int n_treni=(strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;
    
    for(short int i = 0; i < n_treni; i++){ 
       
        treni[i] = fork();
       
        if(treni[i] < 0) {
            fprintf(stderr, "Fork Failed\n");
            exit(EXIT_FAILURE);
        }
        else if(treni[i] == 0) {

            char arg_treno[5];
            sprintf(arg_treno, "%d",i+1);
            execl("./processo_treno", "processo_treno", arg_treno, MODE, NULL);
        }
       
    }

    while(signalCounter < n_treni) {}

    exit(EXIT_SUCCESS);
    return 0;
}

int creaSegmenti() {
    FILE * file; //buffer for file operations
    char name[5];

    for (int i = 1; i <= 16; ++i)
    {   
        //generating the name of the file MAx with metavariables
        sprintf(name, "MA%d",i);

        //opeing the file in write-only mode
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

void trainEndMissionHandler(int signum) {
    if (signum == SIGUSR1)
    {
        printf("Ricevo %d° SIGUSR1!\n", ++signalCounter);
    }
    
}

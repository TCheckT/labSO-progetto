#include "header.h"

int main() {
    // PADRE_TRENI crea i segmenti di binario
    if(creaSegmenti()!=0)
        perror("errore creazione segmenti di binario");

    // PADRE_TRENI genera processi figli PROCESSI_TRENI T1-5
    pid_t T1, T2, T3, T4, T5;

    T1 = fork();
    if(T1 < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(T1 == 0) {
        // treno T1
        routineTreno(1); 
        exit(EXIT_SUCCESS);
    }

    T2 = fork();
    if(T2 < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(T2 == 0) {
        // treno T2
        routineTreno(2); 
        exit(EXIT_SUCCESS);
    }

    T3 = fork();
    if(T3 < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(T3 == 0) {
        // treno T3
        routineTreno(3); 
        exit(EXIT_SUCCESS);
    }

    T4 = fork();
    if(T4 < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(T4 == 0) {
        // treno T4
        routineTreno(4); 
        exit(EXIT_SUCCESS);
    }

    T5 = fork();
    if(T5 < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(T5 == 0) {
        // treno T5
        routineTreno(5); 
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}

int creaSegmenti() {
    FILE *MA1 = fopen("MA1", "w");
    FILE *MA2 = fopen("MA2", "w");
    FILE *MA3 = fopen("MA3", "w");
    FILE *MA4 = fopen("MA4", "w");
    FILE *MA5 = fopen("MA5", "w");
    FILE *MA6 = fopen("MA6", "w");
    FILE *MA7 = fopen("MA7", "w");
    FILE *MA8 = fopen("MA8", "w");
    FILE *MA9 = fopen("MA9", "w");
    FILE *MA10 = fopen("MA10", "w");
    FILE *MA11 = fopen("MA11", "w");
    FILE *MA12 = fopen("MA12", "w");
    FILE *MA13 = fopen("MA13", "w");
    FILE *MA14 = fopen("MA14", "w");
    FILE *MA15 = fopen("MA15", "w");
    FILE *MA16 = fopen("MA16", "w");
    
    // Impostare permessi accesso ai file 666
    int i;
    i = chmod("MA1", 0666); 
    i = chmod("MA2", 0666);
    i = chmod("MA3", 0666);
    i = chmod("MA4", 0666);
    i = chmod("MA5", 0666);
    i = chmod("MA6", 0666);
    i = chmod("MA7", 0666);
    i = chmod("MA8", 0666);
    i = chmod("MA9", 0666);
    i = chmod("MA10", 0666);
    i = chmod("MA11", 0666);
    i = chmod("MA12", 0666);
    i = chmod("MA13", 0666);
    i = chmod("MA14", 0666);
    i = chmod("MA15", 0666);
    i = chmod("MA16", 0666);

    // SCRIVERE 0 come primo carattere di tutti i file
    // scrive 6 byte
    /*char fileInit[1];
    fileInit[0] = 48; // ASCII 48 => 0
    */
    char fileInit[1];
    fileInit[0] = 48;

    fwrite(fileInit, sizeof(char), 1, MA1);
    fwrite(fileInit, sizeof(char), 1, MA2);
    fwrite(fileInit, sizeof(char), 1, MA3);
    fwrite(fileInit, sizeof(char), 1, MA4);
    fwrite(fileInit, sizeof(char), 1, MA5);
    fwrite(fileInit, sizeof(char), 1, MA6);
    fwrite(fileInit, sizeof(char), 1, MA7);
    fwrite(fileInit, sizeof(char), 1, MA8);
    fwrite(fileInit, sizeof(char), 1, MA9);
    fwrite(fileInit, sizeof(char), 1, MA10);
    fwrite(fileInit, sizeof(char), 1, MA11);
    fwrite(fileInit, sizeof(char), 1, MA12);
    fwrite(fileInit, sizeof(char), 1, MA13);
    fwrite(fileInit, sizeof(char), 1, MA14);
    fwrite(fileInit, sizeof(char), 1, MA15);
    fwrite(fileInit, sizeof(char), 1, MA16);
    
    fclose(MA1);
    fclose(MA2);
    fclose(MA3);
    fclose(MA4);
    fclose(MA5);
    fclose(MA6);
    fclose(MA7);
    fclose(MA8);
    fclose(MA9);
    fclose(MA10);
    fclose(MA11);
    fclose(MA12);
    fclose(MA13);
    fclose(MA14);
    fclose(MA15);
    fclose(MA16);
    return 0;
}

int routineTreno(int numeroTreno) {
    printf("treno T%d creato\n", numeroTreno);
}
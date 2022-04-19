#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "header.h"

int main(int argc, char *argv[]) {
    /* printf("il programma si chiama %s\n", argv[0]);
    printf("ed i suoi %i argomenti sono:\n", argc-1);
    for(int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    } */

    if(inputCheck(argc, argv) != 0) {
        return 0;
    }
    printf("Formato input corretto!\n");

    //A questo punto è necessario creare il processo PADRE_TRENI con una fork()
    // come lavoro su tale processo in seguito?

    pid_t PADRE_TRENI;
    printf("Prima della fork. pid = %d, pid del genitore = %d\n",getpid(), getppid());

    PADRE_TRENI = fork();
    if(PADRE_TRENI < 0)
        perror("fork error");
    else if(PADRE_TRENI == 0) {
        //figlio: Qua è dove deve agire il processo figlio del main PADRE_TRENI
        printf("[Figlio] pid = %d, pid del genitore = %d\n",getpid(), getppid());

        // PADRE_TRENI crea i segmenti di binario
        if(creaSegmenti()!=0)
            perror("errore creazione segmenti di binario");

        // PADRE_TRENI genera processi figli PROCESSI_TRENI T1-5
        pid_t T1, T2, T3, T4, T5;

        T1 = fork();
        if(T1 < 0)
            perror("fork error");
        else if(T1 == 0) {
            // treno T1
            routineTreno(1); 
            exit(EXIT_SUCCESS);
        }

        T2 = fork();
        if(T2 < 0)
            perror("fork error");
        else if(T2 == 0) {
            // treno T2
            routineTreno(2); 
            exit(EXIT_SUCCESS);
        }

        T3 = fork();
        if(T3 < 0)
            perror("fork error");
        else if(T3 == 0) {
            // treno T3
            routineTreno(3); 
            exit(EXIT_SUCCESS);
        }

        T4 = fork();
        if(T4 < 0)
            perror("fork error");
        else if(T4 == 0) {
            // treno T4
            routineTreno(4); 
            exit(EXIT_SUCCESS);
        }

        T5 = fork();
        if(T5 < 0)
            perror("fork error");
        else if(T5 == 0) {
            // treno T5
            routineTreno(5); 
            exit(EXIT_SUCCESS);
        }

        exit(EXIT_SUCCESS);


    } else {
        // genitore: Qua è dove continua ad agire il main
        printf("[Genitore] pid = %d, pid del mio genitore = %d\n",getpid(), getppid());
        printf("[Genitore] Mio figlio ha pid = %d\n", PADRE_TRENI);
        sleep(1); // attende 1 secondo
    }
    // entrambi i processi
    printf("PID %d termina.\n", getpid());

    return 0;
}


int inputCheck(int argc, char *argv[]) {

    if(argc < 3 || argc > 4) {
        printf("Sono attesi almeno 2 parametri, al più 3.\n");
        printf("Formato atteso: <./nomeprogramma [ECTS1/ECTS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    /* Input check: formato parametri corretto */
    // primo parametro ECTS1 oppure ECTS2
    if(strcmp(argv[1], "ECTS1") != 0 && strcmp(argv[1], "ECTS2") != 0) {
        printf("%s non accettato come primo parametro.\n", argv[1]);
        printf("Specificare modalità ECTS1 o modalità ECTS2 come primo parametro.\n");
        printf("Formato atteso: <./nomeprogramma [ECTS1/ECTS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    // se primo parametro è ECTS1 il secondo deve essere MAPPA1 o MAPPA2
    if(strcmp(argv[1], "ECTS1") == 0) {
        if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
            printf("Opzione %s non accettata come secondo parametro se si è in modalità ECTS1.\n", argv[2]);
            printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
            printf("Formato atteso: <./nomeprogramma [ECTS1/ECTS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
            return 1;
        }
    }

    // se primo parametro è ECTS2 il secondo può essere RBC e il terzo MAPPA1 o MAPPA2,
    // altrimenti il secondo sarà MAPPA1 o MAPPA2
    if(strcmp(argv[1], "ECTS2") == 0) {
        if(argc == 4) {
            if(strcmp(argv[2], "RBC") != 0 || 
                strcmp(argv[3], "MAPPA1") != 0 && strcmp(argv[3], "MAPPA2") != 0) {
                printf("In modalità ECTS2 ci si aspetta come secondo parametro in input l'eventuale opzione RBC e come terzo parametro la specifica dell'itinerario MAPPA1 o MAPPA2.\n");
                printf("Formato atteso: <./nomeprogramma [ECTS1/ECTS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
        }
        if(argc == 3) {
            if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
                printf("%s non accettato come secondo parametro.\n", argv[2]);
                printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
                printf("Formato atteso: <./nomeprogramma [ECTS1/ECTS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
        }
    }
    return 0;
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
    char fileInit[1];
    fileInit[0] = 48; // ASCII 48 => 0
    fwrite(fileInit, 1, 1, MA1);
    fwrite(fileInit, 1, 1, MA2);
    fwrite(fileInit, 1, 1, MA3);
    fwrite(fileInit, 1, 1, MA4);
    fwrite(fileInit, 1, 1, MA5);
    fwrite(fileInit, 1, 1, MA6);
    fwrite(fileInit, 1, 1, MA7);
    fwrite(fileInit, 1, 1, MA8);
    fwrite(fileInit, 1, 1, MA9);
    fwrite(fileInit, 1, 1, MA10);
    fwrite(fileInit, 1, 1, MA11);
    fwrite(fileInit, 1, 1, MA12);
    fwrite(fileInit, 1, 1, MA13);
    fwrite(fileInit, 1, 1, MA14);
    fwrite(fileInit, 1, 1, MA15);
    fwrite(fileInit, 1, 1, MA16);

    return 0;
}

int routineTreno(int numeroTreno) {
    printf("treno T%d creato\n", numeroTreno);
}
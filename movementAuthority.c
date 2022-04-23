#include "header.h"


int main(int argc, char *argv[]) {

    /* inputCheck controlla che gli argomenti passati 
       in input in argv siano del giusto formato */
    if(inputCheck(argc, argv) != 0) {
        return 0;
    }
    printf("Formato input corretto!\n");

    //A questo punto è necessario creare il processo PADRE_TRENI con una fork()

    pid_t PADRE_TRENI;

    PADRE_TRENI = fork();
    if(PADRE_TRENI < 0) {
        perror("fork error");
        exit(-1);
    }
    else if(PADRE_TRENI == 0) {
        //figlio: Qua è dove deve agire il processo figlio del main PADRE_TRENI
        execl("./padre_treni", 0);
    } else {
        // genitore: Qua è dove continua ad agire il main
        wait(NULL);
        printf("[Genitore] pid = %d, pid del mio genitore = %d\n",getpid(), getppid());
        printf("[Genitore] Mio figlio ha pid = %d\n", PADRE_TRENI);
        sleep(1); // attende 1 secondo
        exit(0);
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
#include "header.h"

int main(int argc, char *argv[]) {

    /* controlloInput controlla che gli argomenti passati 
       in input in argv siano del giusto formato  
       return 0 se input corretto e non RBC
       return 1 se input errato
       return 2 se input corretto con opzione RBC */
    int risultatoControlloInput = controlloInput(argc, argv);
    printf("resInput = %d\n", risultatoControlloInput);
    if(risultatoControlloInput == 1) {
        perror("Formato input errato!\n");
        exit(-1);
    } else if (risultatoControlloInput == 2) {
        printf("Launching server...\n");
        execl("./server_RBC", "server_RBC", argv[3], NULL);
    }
    
    char* MAPPA;

    if(argc == 4) {
        MAPPA = argv[3];
    } else {
        // altrimenti nel secondo
        MAPPA = argv[2];
    }

    char* MODE = argv[1];

    //Crea pipe per invio itinerari dal registro ai treni
    // Treno apre in lettura registro apre in scrittura
    
    char nomePipe[20];
    int n_treni=(strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;

    for (int i = 1; i <= n_treni; ++i)
    {
        sprintf(nomePipe, "T%dregisterPipe", i);
        unlink(nomePipe);
        mknod(nomePipe, S_IFIFO, 0);
        chmod(nomePipe, 0660);
    }
  
    //crea il processo PADRE_TRENI con una fork()

    pid_t PADRE_TRENI;

    PADRE_TRENI = fork();
    if(PADRE_TRENI < 0) {
        fprintf(stderr, "Fork Failed\n");
        exit(EXIT_FAILURE);
    }
    else if(PADRE_TRENI == 0) {
        //figlio: esegue processo padre_treni
        execl("./padre_treni", "padre_treni", MAPPA, MODE, NULL);
        
    } else {
        // genitore: Qua è dove continua movementAuthority

        // altra fork per lanciare il processo registro
        pid_t REGISTRO;
        REGISTRO = fork();
        if(REGISTRO < 0) {
            fprintf(stderr, "Fork Failed\n");
            exit(EXIT_FAILURE);
        }
        else if(REGISTRO == 0) {
            // figlio: esegue processo registro
            execl("./registro", "registro", MAPPA, NULL);
        } else {
            // genitore: Qua è dove continua movementAuthority
            wait(NULL);
            
            sleep(1); // attende 1 secondo
            
        }
        wait(NULL);
    }
    wait(NULL);
    // entrambi i processi

    printf("END OF PROGRAM\n");
    
    return 0;
}


int controlloInput(int argc, char *argv[]) {

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
            return 2;
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
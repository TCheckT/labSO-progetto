#include "header.h"

int main(int argc, char *argv[]) {
    /* inputCheck check that input arguments are in the correct format:
        return 0 if input is correct and RBC option not present
        return 1 if input is wrong
        return 2 if input correct and RBC option is present */
    int inputCheckResult = inputCheck(argc, argv);
    if(inputCheckResult == 1) {
        perror("Formato input errato!\n");
        exit(EXIT_FAILURE);
    } else if (inputCheckResult == 2) {
        // if inputCheckResult == 2 this process will run the server
        printf("Launching server...\n");
        execl("./server_RBC", "server_RBC", argv[3], NULL);
    }

    /* if inputCheckResult = 0 the process continue 
        creating padre_treni and registro */
    
    char* MAPPA = argv[2];
    char* ETCS = argv[1];

    
    // Create pipes that will be used from register to send itinerary to trains 
    char itineraryPipeName[30];
    int numberOfTrains = (strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;

    for (int i = 1; i <= numberOfTrains; ++i)
    {
        sprintf(itineraryPipeName, "T%dregisterPipe", i);
        unlink(itineraryPipeName);
        mknod(itineraryPipeName, S_IFIFO, 0);
        chmod(itineraryPipeName, 0660);
    }
  
    // create process padre_treni
    pid_t PADRE_TRENI;

    PADRE_TRENI = fork();
    if(PADRE_TRENI < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if(PADRE_TRENI == 0) {
        // execute padre_treni
        execl("./padre_treni", "padre_treni", MAPPA, ETCS, NULL);
        
    } else {
        // process movementAuthority continue

        // create process registro
        pid_t REGISTRO;
        REGISTRO = fork();
        if(REGISTRO < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(EXIT_FAILURE);
        }
        else if(REGISTRO == 0) {
            // execute registro
            execl("./registro", "registro", MAPPA, ETCS, NULL);
        } else {
            // process movementAuthority continue

            // wait for registro to terminate 
            wait(NULL);
        }
        // wait for padre_treni to terminate
        wait(NULL);
    }
    printf("END OF PROGRAM\n");
    return 0;
}


int inputCheck(int argc, char *argv[]) {

    // check correct number of arguments
    if(argc < 3 || argc > 4) {
        printf("Sono attesi almeno 2 parametri, al più 3.\n");
        printf("Formato atteso: <./nomeprogramma [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    // check that first argument is ETCS1 or ETCS2
    if(strcmp(argv[1], "ETCS1") != 0 && strcmp(argv[1], "ETCS2") != 0) {
        printf("%s non accettato come primo parametro.\n", argv[1]);
        printf("Specificare modalità ETCS1 o modalità ETCS2 come primo parametro.\n");
        printf("Formato atteso: <./nomeprogramma [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    // if first argument is ETCS1, second one must be MAPPA1 or MAPPA2
    if(strcmp(argv[1], "ETCS1") == 0) {
        if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
            printf("Opzione %s non accettata come secondo parametro se si è in modalità ECTS1.\n", argv[2]);
            printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
            printf("Formato atteso: <./nomeprogramma [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
            return 1;
        }
    }

    // if first argument is ETCS2, second one can be RBC, and in that case third one 
    // must be MAPPA1 or MAPPA2. Otherwise the second one must be MAPPA1 or MAPPA2
    if(strcmp(argv[1], "ETCS2") == 0) {
        if(argc == 4) {
            if(strcmp(argv[2], "RBC") != 0 || 
                strcmp(argv[3], "MAPPA1") != 0 && strcmp(argv[3], "MAPPA2") != 0) {
                printf("In modalità ETCS2 ci si aspetta come secondo parametro in input l'eventuale opzione RBC e come terzo parametro la specifica dell'itinerario MAPPA1 o MAPPA2.\n");
                printf("Formato atteso: <./nomeprogramma [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
            return 2;
        }
        if(argc == 3) {
            if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
                printf("%s non accettato come secondo parametro.\n", argv[2]);
                printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
                printf("Formato atteso: <./nomeprogramma [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
        }
    }
    return 0;
}
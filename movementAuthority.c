#include "header.h"

/* Function to check if arguments passed to movementAuthority are in the correct format */
int inputCheck(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    
    switch(inputCheck(argc, argv)){
        case 1:
            printf("Input not accepted! See suggestions and try again.\n");
            return 0;
            break;
        case 2:
            printf("Launching server...\n");
            execl("./server_RBC", "server_RBC", argv[3], NULL);
            break;
        default:break;    
    }

    const char* MAPPA = argv[2];
    const char* ETCS = argv[1];
    
    /* Create pipes that will be used from register to send itineraries to trains */
    char itineraryPipeName[30];
    const int numberOfTrains = (strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;

    for (int i = 1; i <= numberOfTrains; ++i)
    {
        sprintf(itineraryPipeName, "T%dregisterPipe", i);
        unlink(itineraryPipeName);
        mknod(itineraryPipeName, S_IFIFO, 0);
        chmod(itineraryPipeName, 0660);
    }
  
    /* create process padre_treni */
    pid_t PADRE_TRENI;

    PADRE_TRENI = fork();
    if(PADRE_TRENI < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if(PADRE_TRENI == 0) {
        // exec padre_treni with MAPPA and ETCS as arguments
        printf("movementAuthority: exec padre_treni\n");
        execl("./padre_treni", "padre_treni", MAPPA, ETCS, NULL);
        
    } else {
        /* process movementAuthority continues */

        /* create process registro */
        pid_t REGISTRO;

        REGISTRO = fork();
        if(REGISTRO < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(EXIT_FAILURE);
        }
        else if(REGISTRO == 0) {
            // exec registro with MAPPA and ETCS as arguments
            printf("movementAuthority: exec registro\n");
            execl("./registro", "registro", MAPPA, ETCS, NULL);
        } else {
            /* process movementAuthority continues */

            /* wait for registro to terminate */
            wait(NULL);
        }
        /* wait for padre_treni to terminate */
        wait(NULL);
    }
    printf("\nmovementAuthority: process terminated...\nEND OF PROGRAM!\n");
    return 0;
}

/* inputCheck check that input arguments are in the correct format:
    return 0 if input is correct and RBC option not present
    return 1 if input is wrong
    return 2 if input correct and RBC option is present */
int inputCheck(int argc, char *argv[]) {

    // Check correct number of arguments
    if(argc < 3 || argc > 4) {
        printf("Sono attesi almeno 2 parametri, al pi?? 3.\n");
        printf("Formato atteso: <./movementAuthority [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    // Check that first argument is ETCS1 or ETCS2
    if(strcmp(argv[1], "ETCS1") != 0 && strcmp(argv[1], "ETCS2") != 0) {
        printf("%s non accettato come primo parametro.\n", argv[1]);
        printf("Specificare modalit?? ETCS1 o modalit?? ETCS2 come primo parametro.\n");
        printf("Formato atteso: <./movementAuthority [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
        return 1;
    }

    // if first argument is ETCS1, second one must be MAPPA1 or MAPPA2
    if(strcmp(argv[1], "ETCS1") == 0) {
        if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
            printf("Opzione %s non accettata come secondo parametro se si ?? in modalit?? ECTS1.\n", argv[2]);
            printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
            printf("Formato atteso: <./movementAuthority [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
            return 1;
        }
    }

    // if first argument is ETCS2, second one can be RBC, and in that case third one 
    // must be MAPPA1 or MAPPA2. Otherwise the second one must be MAPPA1 or MAPPA2
    if(strcmp(argv[1], "ETCS2") == 0) {
        if(argc == 4) {
            if(strcmp(argv[2], "RBC") != 0 || 
                strcmp(argv[3], "MAPPA1") != 0 && strcmp(argv[3], "MAPPA2") != 0) {
                printf("In modalit?? ETCS2 ci si aspetta come secondo parametro in input l'eventuale opzione RBC e come terzo parametro la specifica dell'itinerario MAPPA1 o MAPPA2.\n");
                printf("Formato atteso: <./movementAuthority [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
            return 2;
        }
        if(argc == 3) {
            if(strcmp(argv[2], "MAPPA1") != 0 && strcmp(argv[2], "MAPPA2") != 0) {
                printf("%s non accettato come secondo parametro.\n", argv[2]);
                printf("Specificare itinerario MAPPA1 o itinerario MAPPA2 come secondo parametro.\n");
                printf("Formato atteso: <./movementAuthority [ETCS1/ETCS2][OPZIONE_RBC][MAPPA1/MAPPA2]>\n");
                return 1;
            }
        }
    }
    return 0;
}
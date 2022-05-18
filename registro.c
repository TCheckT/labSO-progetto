#include "header.h"

int main(int argc, char *argv[]) {
    printf("Ricevo %s da movementAuthority\n", argv[1]);
    
    char* MAPPA = argv[1];
    int numeroTreni;
    if (strcmp(MAPPA, "MAPPA1") == 0) {
        numeroTreni = 4;
    } else if (strcmp(MAPPA, "MAPPA2") == 0) {
        numeroTreni = 5;
    } else {
        printf("Error: MAPPA not recognized\n");
    }
    
    char * M1itinerarioT1[] = {"S1", "MA1", "MA2", "MA3", "MA8", "S6"};
    char * M1itinerarioT2[] = {"S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6"};
    char * M1itinerarioT3[] = {"S7", "MA13", "MA12", "MA11", "MA10", "MA9", "S3"};
    char * M1itinerarioT4[] = {"S4", "MA14", "MA15", "MA16", "MA12", "S8"};
    char * M1itinerarioT5[] = {};


    char * M2itinerarioT1[] = {"S2", "MA5", "MA7", "MA3", "MA8", "S6"};
    char * M2itinerarioT2[] = {"S3", "MA9", "MA10", "MA11", "MA12", "S8"};
    char * M2itinerarioT3[] = {"S4", "MA14", "MA15", "MA16", "MA12", "S8"};
    char * M2itinerarioT4[] = {"S6", "MA8", "MA3", "MA2", "MA1", "S1"};
    char * M2itinerarioT5[] = {"S5", "MA4", "MA3", "MA2", "MA1", "S1"};
    
    
    // Dopo aver creato le tabelle il processo registro può connettersi in lettura alla PIPE
    // itineraryRequestPipe, in attesa di ricevere richieste di itinerari.
    int irp_fd;
    char richiesta[100];
    int richiesteSoddisfatte = 0;

    sleep(2);
    while(richiesteSoddisfatte < numeroTreni) { // se MAPPA1 < 4 se MAPPA2 < 5
        
        //Crea pipe per invio richieste itinerario dei treni al registro
        // Treno apre in scrittura registro apre in lettura
        unlink("itineraryRequestPipe");
        mknod("itineraryRequestPipe", S_IFIFO, 0);
        chmod("itineraryRequestPipe", 0660);

        irp_fd = open("itineraryRequestPipe", O_RDONLY);
        printf("waiting for requests...\n");

        attesaRichieste(irp_fd, richiesta);
        printf("Request for itinerary from %s received\n", richiesta);

        int i;
        i = assegnaItinerario(richiesta);
        printf("sto per inviare a T%d\n", i);

        if(strcmp(MAPPA, "MAPPA1") == 0) {
            if(i==1) inviaItinerario(M1itinerarioT1, 1, SIZEOF(M1itinerarioT1));
            else if(i==2) inviaItinerario(M1itinerarioT2, 2, SIZEOF(M1itinerarioT2));
            else if(i==3) inviaItinerario(M1itinerarioT3, 3, SIZEOF(M1itinerarioT3));
            else if(i==4) inviaItinerario(M1itinerarioT4, 4, SIZEOF(M1itinerarioT4));
            else if(i==5) inviaItinerario(M1itinerarioT5, 5, SIZEOF(M1itinerarioT5));
            else printf("Errore:richiesta inaspettata");
        } else if (strcmp(MAPPA, "MAPPA2") == 0) {
            if(i==1) inviaItinerario(M2itinerarioT1, 1, SIZEOF(M2itinerarioT1));
            else if(i==2) inviaItinerario(M2itinerarioT2, 2, SIZEOF(M2itinerarioT2));
            else if(i==3) inviaItinerario(M2itinerarioT3, 3, SIZEOF(M2itinerarioT3));
            else if(i==4) inviaItinerario(M2itinerarioT4, 4, SIZEOF(M2itinerarioT4));
            else if(i==5) inviaItinerario(M2itinerarioT5, 5, SIZEOF(M2itinerarioT5));
            else printf("Errore:richiesta inaspettata");
        } else {
            printf("Error: MAPPA not recognized\n");
        }

        richiesteSoddisfatte++;

        printf("%d richieste soddisfatte\n", richiesteSoddisfatte);
        close(irp_fd);
        unlink("itineraryRequestPipe");
    }
    
    exit(EXIT_SUCCESS);
    return 0;
}

// metodo per leggere ripetutamente da pipe e, se richiesta rilevata, chiamare metodo per inviare itinerario
int attesaRichieste(int fd, char *str) { 
    int n;
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
    } while (n > 0 && *str++ != 0);
    return (n > 0); /* Return false if end-of-input */
}

int inviaItinerario(char* itinerario[], int r, int lunghezzaItinerario) {
    int sendingToTrain_fd, lunghezzaTappa, i;// char message [100];

    char nomePipe[20];

    sprintf(nomePipe,"T%dregisterPipe", r);

    printf("Using %s\n", nomePipe);
    
    
    do { /* Continua a provare ad aprire la pipe */
        sendingToTrain_fd = open (nomePipe, O_WRONLY); /* Apre pipe in scrittura */  //HARDCODE da rimuovere, selezionare nome pipe
        if (sendingToTrain_fd == -1) sleep (1); /* Prova ancora dopo 1 secondo */
    } while (sendingToTrain_fd == -1);

    for (i = 0; i < lunghezzaItinerario; i++) { /* Invia tutte le tappe */
    /* Scrive nella pipe */
        char* tappa = itinerario[i];
        lunghezzaTappa = strlen(tappa) + 1;
        printf("Sending %s through pipe\n", tappa);
        write(sendingToTrain_fd, tappa, lunghezzaTappa);

        sleep (1); /* Pause a while */
    }
    close(sendingToTrain_fd);
    return 0;
}

int assegnaItinerario(char * request){
    if(strcmp(request, "T1") == 0)          return 1;
    else if (strcmp(request, "T2") == 0)    return 2;
    else if (strcmp(request, "T3") == 0)    return 3;
    else if (strcmp(request, "T4") == 0)    return 4;
    else if (strcmp(request, "T5") == 0)    return 5;
    else                                    return -1;  
}

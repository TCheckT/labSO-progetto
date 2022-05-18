#include "header.h"

int main(int argc, char *argv[]) {
    printf("Ricevo %s da movementAuthority\n", argv[1]);
    
    char * itinerarioT1[] = {"S1", "MA1", "MA2", "MA3", "MA8", "S6"};
    char * itinerarioT2[] = {"S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6"};
    char * itinerarioT3[] = {"S7", "MA13", "MA12", "MA11", "MA10", "MA9", "S3"};
    char * itinerarioT4[] = {"S4", "MA14", "MA15", "MA16", "MA12", "S8"};

    char * itinerarioT5[] = {};
    // Dopo aver creato le tabelle il processo registro può connettersi in lettura alla PIPE
    // itineraryRequestPipe, in attesa di ricevere richieste di itinerari.
    int irp_fd;
    char richiesta[100];
    int richiesteSoddisfatte = 0;

   

    sleep(2);
    while(richiesteSoddisfatte < 4) { // se MAPPA1 < 4 se MAPPA2 < 5
        
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

        if(i==1) inviaItinerario(itinerarioT1, 1, SIZEOF(itinerarioT1));
        else if(i==2) inviaItinerario(itinerarioT2, 2, SIZEOF(itinerarioT2));
        else if(i==3) inviaItinerario(itinerarioT3, 3, SIZEOF(itinerarioT3));
        else if(i==4) inviaItinerario(itinerarioT4, 4, SIZEOF(itinerarioT4));
        else if(i==5) inviaItinerario(itinerarioT5, 5, SIZEOF(itinerarioT5));
        else printf("Errore:richiesta inaspettata");

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

/*
 char* tappa1; 
    char* tappa2;
    char* tappa3;
    char* tappa4;
    char* tappa5;
    char* tappa6;
    char* tappa7;

    // Popolamento MAPPA1
    tappa1 = "S1";
    tappa2 = "MA1";
    tappa3 = "MA2";
    tappa4 = "MA3";
    tappa5 = "MA8";
    tappa6 = "S6";

    itinerario M1T1 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6};

    tappa1 = "S2";
    tappa2 = "MA5";
    tappa3 = "MA6";
    tappa4 = "MA7";
    tappa5 = "MA3";
    tappa6 = "MA8";
    tappa7 = "S6";
    itinerario M1T2 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6, tappa7};
    
    tappa1 = "S7";
    tappa2 = "MA13";
    tappa3 = "MA12";
    tappa4 = "MA11";
    tappa5 = "MA10";
    tappa6 = "MA9";
    tappa7 = "S3";
    itinerario M1T3 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6, tappa7};
    
    tappa1 = "S4";
    tappa2 = "MA14";
    tappa3 = "MA15";
    tappa4 = "MA16";
    tappa5 = "MA12";
    tappa6 = "S8";
    itinerario M1T4 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6, tappa7};
    
    itinerario M1T5 = {};

     struct Tabella Mappa1 = {M1T1, M1T2, M1T3, M1T4, M1T5};

    
    printf("MAPPA1:\n");
    printf("T4 tappa6: %s\n", M1T4[5]);
    printf("T3 tappa1: %s\n", M1T3[0]);
    printf("T2 tappa3: %s\n", M1T2[2]);
    printf("T1 tappa5: %s\n", M1T1[4]);
    
    printf("T4 tappa 4: %s\n", Mappa1.T4[3]);
    
    printf("T1 tappa 3: %s\n", Mappa1.T3[2]);
    

    // popolamento MAPPA2
    tappa1 = "S2";
    tappa2 = "MA5";
    tappa3 = "MA6";
    tappa4 = "MA7";
    tappa5 = "MA3";
    tappa6 = "MA8";
    tappa7 = "S6";
    itinerario M2T1 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6, tappa7};

    tappa1 = "S3";
    tappa2 = "MA9";
    tappa3 = "MA10";
    tappa4 = "MA11";
    tappa5 = "MA12";
    tappa6 = "S8";
    itinerario M2T2 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6};

    tappa1 = "S4";
    tappa2 = "MA14";
    tappa3 = "MA15";
    tappa4 = "MA16";
    tappa5 = "MA12";
    tappa6 = "S8";
    itinerario M2T3 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6};
    
    tappa1 = "S6";
    tappa2 = "MA8";
    tappa3 = "MA3";
    tappa4 = "MA2";
    tappa5 = "MA1";
    tappa6 = "S1";
    itinerario M2T4 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6};

    tappa1 = "S5";
    tappa2 = "MA4";
    tappa3 = "MA3";
    tappa4 = "MA2";
    tappa5 = "MA1";
    tappa6 = "S1";
    itinerario M2T5 = {tappa1, tappa2, tappa3, tappa4, tappa5, tappa6};
    
    struct Tabella Mappa2 = {M2T1, M2T2, M2T3, M2T4, M2T5};

    printf("MAPPA2:\n");
    printf("T4 tappa6: %s\n", M2T4[5]);
    printf("T3 tappa1: %s\n", M2T3[0]);
    printf("T2 tappa3: %s\n", M2T2[2]);
    printf("T1 tappa5: %s\n", M2T1[4]);
    
    printf("T4 tappa 4: %s\n", Mappa2.T4[3]);
    
    printf("T1 tappa 3: %s\n", Mappa2.T3[2]);
    printf("T1 tappa 3: %s\n", M2T1[2]);

*/
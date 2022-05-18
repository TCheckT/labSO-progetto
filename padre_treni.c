#include "header.h"

int main(int argc, char *argv[]) {

    char* MAPPA = argv[1];
    // PADRE_TRENI crea i segmenti di binario
    if(creaSegmenti()!=0)
        perror("errore creazione segmenti di binario\n");

    // PADRE_TRENI genera processi figli PROCESSI_TRENI T1-5
    pid_t T1, T2, T3, T4, T5;

    T1 = fork();
    if(T1 < 0) {
        fprintf(stderr, "Fork Failed\n");
        exit(EXIT_FAILURE);
    }
    else if(T1 == 0) {
       /*
        // treno T1
        int irp_fd, lunghezzaRichiesta; char richiesta [100];
        sprintf(richiesta,"T1"); // Prepara richiesta 
        lunghezzaRichiesta = strlen (richiesta) + 1;
        do { // Prova ad aprire la pipe fino a che non ha successo 
            irp_fd = open ("itineraryRequestPipe", O_WRONLY); // Apre la pipe con nome
            if (irp_fd == -1) sleep (1); // Prova ancora dopo un secondo se fallisce
        } while (irp_fd == -1);
        // invio richiesta
        write (irp_fd, richiesta, lunghezzaRichiesta);

        // T1 riceve itinerario
        int T1rp_fd;
        char tappaRicevuta[100];
        char itinerary[6][5];
        int i = 0;
        T1rp_fd = open("T1registerPipe", O_RDONLY);

        while(riceviTappe(T1rp_fd, tappaRicevuta)) {
            //salvare le tappe in una propria struttura dati
            printf("Saving %s into T1\n", tappaRicevuta);
            strcpy(itinerary[i], tappaRicevuta);
            i++;
        }
        close(T1rp_fd);
        unlink("T1registerPipe");

        printf("T1:");
        for (int i = 0; i < 6; ++i) printf("->%s", itinerary[i]);
        printf("\n");
        
       */
        
        routineTreno(1); 
        exit(EXIT_SUCCESS);
    }
    wait(NULL);

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
    wait(NULL);

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
    wait(NULL);

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
    wait(NULL);

    if(strcmp(MAPPA, "MAPPA2") == 0) {
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
    }
    wait(NULL);
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

int routineTreno(int numeroTreno) {
    
    int irp_fd, lunghezzaRichiesta; char richiesta [100];
    sprintf(richiesta,"T%d", numeroTreno); // Prepara richiesta 
    lunghezzaRichiesta = strlen (richiesta) + 1;

    do { // Prova ad aprire la pipe fino a che non ha successo 
        irp_fd = open ("itineraryRequestPipe", O_WRONLY); // Apre la pipe con nome
        if (irp_fd == -1) sleep (1); // Prova ancora dopo un secondo se fallisce
    } while (irp_fd == -1);

    // invio richiesta
    write (irp_fd, richiesta, lunghezzaRichiesta);

    // Treno riceve itinerario
    int Trp_fd;
    char tappaRicevuta[100];
    char itinerario[10][5];
    int i = 0;
    char nomePipe[20];
    sprintf(nomePipe, "T%dregisterPipe", numeroTreno);
    Trp_fd = open(nomePipe, O_RDONLY);

    while(riceviTappe(Trp_fd, tappaRicevuta)) {
        //salvare le tappe in una propria struttura dati
        printf("Saving %s into T%d\n", tappaRicevuta, numeroTreno);
        strcpy(itinerario[i], tappaRicevuta);
        i++;
    }
    close(Trp_fd);
    unlink(nomePipe);

    printf("T%d:", numeroTreno);
    for (int j = 0; j < i; ++j) printf("->%s", itinerario[j]);
    printf("\n");
    printf("treno T%d ready\n", numeroTreno);
    printf("\n");

/*
    // creazione di una variabile che conterrà il nome del treno
    char * nomeTreno;
    sprintf(nomeTreno, "T%d", numeroTreno);
    
    int irp_fd, lunghezzaRichiesta; char richiesta [100];
    strcpy(richiesta,nomeTreno); // Prepara richiesta 
    lunghezzaRichiesta = strlen (richiesta) + 1;
    do { // Prova ad aprire la pipe fino a che non ha successo 
        irp_fd = open ("itineraryRequestPipe", O_WRONLY); // Apre la pipe con nome
        if (irp_fd == -1) sleep (1); // Prova ancora dopo un secondo se fallisce
    } while (irp_fd == -1);
    // invio richiesta
    write (irp_fd, richiesta, lunghezzaRichiesta);

    // T1 riceve itinerario
    int rp_fd;
    char tappaRicevuta[100];
    char itinerary[6][5];
    int i = 0;
    char * nomePipe;

    sprintf(nomePipe,"%sregisterPipe",nomeTreno);

    rp_fd = open(nomePipe, O_RDONLY);

    while(riceviTappe(rp_fd, tappaRicevuta)) {
        //salvare le tappe in una propria struttura dati
        printf("Saving %s into %s\n", tappaRicevuta, nomeTreno);
        strcpy(itinerary[i], tappaRicevuta);
        i++;
    }
    close(rp_fd);
    unlink(nomePipe);

    printf("%s:", nomeTreno);
    for (int i = 0; i < 6; ++i) printf("->%s", itinerary[i]);
    printf("\n");
*/
    return 0;
}

int riceviTappe(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}
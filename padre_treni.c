#include "header.h"

int creaSegmenti(); 
char * genName(int);

int requestScanner(int fd, char *str) { 
    int n;
    printf("Arrivo qui?\n");
    do { /* Read characters until ’\0’ or end-of-input */
        n = read (fd, str, 1); /* Read one character */
        printf("Mi blocco qui?\n");
        sleep(2);
    } while (n > 0 && *str++ != 0);
    return (n > 0); /* Return false if end-of-input */
}

int readSteps(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}

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
        int fd, messageLen; char message [100];
        sprintf(message,"T1"); /* Prepare message */
        messageLen = strlen (message) + 1;
        do { /* Keep trying to open the file until successful */
            fd = open ("itineraryRequestPipe", O_WRONLY); /* Open named pipe for writing */
            if (fd == -1) sleep (1); /* Try again in 1 second */
        } while (fd == -1);
        // invio richiesta
        write (fd, message, messageLen);

        // T1 riceve itinerario
        int fd2;
        char received[100];
        char* itinerary[6];
        int i = 0;
        fd2 = open("T1registerPipe", O_RDONLY);

        while(readSteps(fd2, received)) {
            //salvare le tappe in una propria struttura dati
            printf("saving %s into T1\n", received);
            itinerary[i] =  received;
            i++;
        }
        close(fd2);
        unlink("T1registerPipe");

        // QUA QUA QUA QUA QUA QUA
        printf("%s, %s, %s, %s, %s, %s\n", itinerary[0], itinerary[1], itinerary[2], itinerary[3], itinerary[4], itinerary[5]);

        sleep (3);
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
    printf("treno T%d ready\n", numeroTreno);
    return 0;
}
#include "header.h"

int main(int argc, char const *argv[])
{
	int irp_fd, lunghezzaRichiesta; char richiesta [100];
    
	sprintf(richiesta, "T%s", argv[1]);

    printf("Processo %s iniziato\n", richiesta);

    lunghezzaRichiesta = strlen (richiesta) + 1;

    char nomeRequestPipe[30];
    sprintf(nomeRequestPipe, "T%sitineraryRequestPipe", argv[1]);
    do { // Prova ad aprire la pipe fino a che non ha successo 
        irp_fd = open (nomeRequestPipe, O_WRONLY); // Apre la pipe con nome
        printf("%s prova ad aprire %s, risultato: %d\n", richiesta, nomeRequestPipe, irp_fd);
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
    sprintf(nomePipe, "T%sregisterPipe", argv[1]);
    Trp_fd = open(nomePipe, O_RDONLY);

    while(riceviTappe(Trp_fd, tappaRicevuta)) {
        //salvare le tappe in una propria struttura dati
        printf("Saving %s into T%s\n", tappaRicevuta, argv[1]);
        strcpy(itinerario[i], tappaRicevuta);
        i++;
    }
    close(Trp_fd);
    unlink(nomePipe);

    printf("T%s:", argv[1]);
    for (int j = 0; j < i; ++j) printf("->%s", itinerario[j]);
    printf("\n");
    printf("treno T%s ready\n", argv[1]);
    printf("\n");

    int count = 0;
    while(count < 2){
        count++;
        printf("##########T%s is doing stuff number %d... \n", argv[1], count);
        sleep(1);    
    }
    
    kill(getppid(), SIGUSR1);

    return 0;
}

int riceviTappe(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}
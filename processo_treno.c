#include "header.h"

int main(int argc, char const *argv[])
{
	int irp_fd, lunghezzaRichiesta; char richiesta [100];
    
	sprintf(richiesta, "T%s", argv[1]);

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

    return 0;
}

int riceviTappe(int fd, char *str){
    int n;
    do {
        n = read(fd, str, 1);
    } while(n > 0 && *str++ != '\0');
    return (n > 0);
}
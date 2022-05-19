#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> /* For S_IFIFO */
#include <fcntl.h>

#define SIZEOF(a) ( sizeof a / sizeof a[0] )

typedef char* itinerario[10]; 

struct Tabella {
    char** T1;
    char** T2;
    char** T3;
    char** T4;
    char** T5;
};
//movementAuthority
int controlloInput(int argc, char *argv[]);
int creaSegmenti();
char * genName(int);

//padre_treni
int riceviTappe(int fd, char *str);
int routineTreno(int numeroTreno);

//registro
int attesaRichieste(int fd, char *str);
int inviaItinerario(char* itinerario[], int numeroTreno, int lunghezzaItinerario);
int assegnaItinerario(char[]);

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

typedef char* itinerario[10]; 

struct Tabella {
    char** T1;
    char** T2;
    char** T3;
    char** T4;
    char** T5;
};

int inputCheck(int argc, char *argv[]);
int creaSegmenti();
int routineTreno(int numeroTreno);

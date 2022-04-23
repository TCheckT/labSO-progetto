#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


int inputCheck(int argc, char *argv[]);
int creaSegmenti();
int routineTreno(int numeroTreno);

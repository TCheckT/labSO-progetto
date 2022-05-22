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
#include <signal.h>
#include <time.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>


#define SIZEOF(a) ( sizeof a / sizeof a[0] )

//movementAuthority
int inputCheck(int argc, char *argv[]);
int createTracks();
char * genName(int);

//padre_treni
int receiveStage(int fd, char *str);
int routineTreno(int numeroTreno);

//registro
int waitForRequest(int fd, char *str);
int sendItinerary(char* itinerario[], int numeroTreno, int lunghezzaItinerario);
int assignItinerary(char[]);

//processo_treno
int requestAccessTo(char step[5], const char* MODE);

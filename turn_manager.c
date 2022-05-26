#include "header.h"

int main(int argc, char const *argv[]) {
    /* This process will alternate the content of a file, writing a different
        number in it every second cycling from 1 to 4 or 5 according to MAPPA */
    
    const char* MAPPA = argv[1];

    int numberOfTrains=(strcmp(MAPPA, "MAPPA1") == 0) ? 4 : 5;
    
    /* Create FILE for trains turn alternation and set permissions */
    FILE* trackFilesGuard;
    int turn = 0;
    trackFilesGuard = fopen("trackFilesGuard", "w");
    chmod("trackFilesGuard", 0644);
    fclose(trackFilesGuard);

    while(1) { /* Loop until movementAuthority process send SIGINT */
        trackFilesGuard = fopen("trackFilesGuard", "w");
        fseek(trackFilesGuard, 0, SEEK_SET);
        char nextTurn[2];
        // put turn here
        sprintf(nextTurn, "%d", (turn%numberOfTrains)+1);       
        fwrite(nextTurn, sizeof(char), 1, trackFilesGuard);
        // printf("turn_manager: turn T%s\n", nextTurn);
        fclose(trackFilesGuard);
        turn = (turn%numberOfTrains)+1;
        sleep(1);
    }
    return 0;   
}
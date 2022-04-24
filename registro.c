#include "header.h"
// prende in input numero treno (T1-2-3-4-5) e mappa in uso (MAPPA1-2)

typedef char* itinerario[10]; 

struct Tabella {
    char** T1;
    char** T2;
    char** T3;
    char** T4;
    char** T5;
}; 

int main() {
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

    /* DEBUG printf: */
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

    /* DEBUG printf: */
    printf("MAPPA2:\n");
    printf("T4 tappa6: %s\n", M2T4[5]);
    printf("T3 tappa1: %s\n", M2T3[0]);
    printf("T2 tappa3: %s\n", M2T2[2]);
    printf("T1 tappa5: %s\n", M2T1[4]);
    
    printf("T4 tappa 4: %s\n", Mappa2.T4[3]);
    
    printf("T1 tappa 3: %s\n", Mappa2.T3[2]);
    printf("T1 tappa 3: %s\n", M2T1[2]);
    
    return 0;
}
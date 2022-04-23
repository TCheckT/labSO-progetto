#include "header.h"
// prende in input numero treno (T1-2-3-4-5) e mappa in uso (MAPPA1-2)

typedef char tappa[4];
typedef tappa itinerario[10]; 

struct Tabella {
    itinerario T1;
    itinerario T2;
    itinerario T3;
    itinerario T4;
    itinerario T5;
}; 

int main() {
    // popola Mappa1 e Mappa2 con gli itinerari di ogni treno
    struct Tabella Mappa1, Mappa2;
    struct Tabella *pMappa1;
    pMappa1 = &Mappa1;
    //S1, MA1, MA2, MA3, MA8, S6
    
    tappa *p = NULL;
    p = (tappa *) calloc(6, sizeof(tappa));
    
    return 0;
}
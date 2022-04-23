# labSO-progetto
Progetto per la parte di laboratorio del corso di Sistemi Operativi A.A. 2021-2022

Per ora ci sono due file .c, compilare entrambi con
cc padre_treni.c -o padre_treni
e
cc main.c -o main
ed eseguire ./main, che a sua volta eseguirà ./padre_treni con una exec

//DONE:
- scansione argomenti input e controllo formato [ECTS1/2][PBR][MAPPA1/2]
- creazione processo PADRE_TRENI come figlio processo main
- PADRE_TRENI crea i file rappresentanti i segmenti di binario, imposta correttamente i permessi a 666, ma nel provare ad inserire come primo carattere 0 ne inserisce 6 perchè C è bello e fa magheggi col fatto che sto aggiungendo un char che probabilmente è di 6 byte. W il C sempre sia lodato!!! Mortacci sua...
- PADRE_TRENI fa anche le fork per generare i treni figli ma non so se è corretto farle sequenziali in questo modo.


//TODO:
1- DIFFERENZIARE PROCESSI FIGLI LANCIANDO PROCESSI CON EXEC 

Problemi: 
- è possibile creare l'eseguibile padre_treni nel makefile, mentre viene creato anche l'eseguibile main?
- per processi_treni occorre creare un file eseguibile ciascuno?

2- CREARE PROCESSO REGISTRO CON STRUTTURA DATI PER MAPPA1 E MAPPA2

3- far comunicare rispettiva MAPPA1/MAPPA2 da REGISTRO a ogni treno

4- risistemare header


/

# labSO-progetto
Progetto per la parte di laboratorio del corso di Sistemi Operativi A.A. 2021-2022

Per ora ci sono tre file .c:

Compilare padre_treni.c, registro.c e movementAuthority.c con
cc padre_treni.c -o padre_treni
e
cc registro.c -o registro
e
cc movementAuthority.c -o movementAuthority
ed eseguire ./movementAuthority, che a sua volta eseguirà ./padre_treni e ./registro con una exec

// 16.05.2022
Adesso è disponibile un makefile funzionante. Opzioni per la compilazione:
- "make" per compilare i tre file .c, dopodichè lanciare il programma movementAuthority. Tutti i file creati durante l'esecuzione, come pipe e file binari, saranno creati nella current directory, rendendo il tutto abbastanza disordinato. Usare "make clean" per ripristinare la situazione iniziale
- "make install" per compilare i tre file .c e organizzare la directory in modo che gli eseguibili vengano inseriti nella cartella bin, i file .c nella cartella source e si possano inoltre creare le cartelle nelle quali organizzare i file. 


//DONE:
- scansione argomenti input e controllo formato [ECTS1/2][PBR][MAPPA1/2]
- creazione processo PADRE_TRENI come figlio processo main
- PADRE_TRENI crea i file rappresentanti i segmenti di binario, imposta correttamente i permessi a 666, ma nel provare ad inserire come primo carattere 0 ne inserisce 6 perchè C è bello e fa magheggi col fatto che sto aggiungendo un char che probabilmente è di 6 byte. W il C sempre sia lodato!!! Mortacci sua... [EDIT: RISOLTO!!!!!]
- PADRE_TRENI fa anche le fork per generare i treni figli ma non so se è corretto farle sequenziali in questo modo.
- aggiunto file registro.c che crea 2 strutture dati di tipo Tabella rappresentanti MAPPA1 e MAPPA2 e le popola. è facile accedere agli itinerari dei singoli treni quando richiesto con Mappa[1-2].T[1-5].

- rendere il processo registro figlio del main, da eseguire con fork dopo la creazione delle pipe 
- makefile
- metodo per far richiedere dal treno il proprio itinerario al registro

//TODO:
1- DIFFERENZIARE PROCESSI FIGLI LANCIANDO PROCESSI CON EXEC 

2- CREARE PROCESSO REGISTRO CON STRUTTURA DATI PER MAPPA1 E MAPPA2
Problemi:
- come strutturare le mappe? [DONE]
- come far interagire registro.c con gli altri programmi? [PIPE]
- come comunicarle ai treni? [PIPE]

Si sceglie di far comunicare ai treni i rispettivi itinerari tramite Pipe con nome.
Nel main saranno creati 6 file pipe, uno per ciascun treno, per mandare il rispettivo itinerario dal registro al treno, e il sesto per far inviare le richieste di itinerario dai treni al registro. 
Ogni treno invierà nella pipe itineraryRequestPipe il messaggio (esempio per treno 1): "T1".
Registro riceverà il messaggio e preparerà l'itinerario per poterlo mandare al treno che lo ha richiesto.
Metodi ausiliari per evitare duplicazione di codice: 
- requestItinerary(int numeroTreno) che ogni treno chiamerà come primo metodo. Comportamento: invia il messaggio di richiesta al registro.
- sendItinerary(...) invia sulla pipe corrispondente, in ordine, le tappe dell'itinerario richiesto
Manca da implementare sendItineray, differenziando la pipe in base al treno richiedente, e implementare
il meccanismo per gli altri processi treno.

3- far comunicare rispettiva MAPPA1/MAPPA2 da REGISTRO a ogni treno

4- risistemare header


/

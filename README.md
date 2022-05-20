# labSO-progetto
Progetto per la parte di laboratorio del corso di Sistemi Operativi A.A. 2021-2022

// 16.05.2022
Adesso è disponibile un makefile funzionante. Opzioni per la compilazione:
- "make" per compilare i tre file .c, dopodichè lanciare il programma movementAuthority. Tutti i file creati durante l'esecuzione, come pipe e file binari, saranno creati nella current directory, rendendo il tutto abbastanza disordinato. Usare "make clean" per ripristinare la situazione iniziale
- [NOPE] NON USARE!!!! "make install" per compilare i tre file .c e organizzare la directory in modo che gli eseguibili vengano inseriti nella cartella bin, i file .c nella cartella source e si possano inoltre creare le cartelle nelle quali organizzare i file. 

// 21.05.2022
!!!!!!!!!!!!Aggiornare qua README quanto prima!!!!!!!!!!!!!

//DONE:
- scansione argomenti input e controllo formato [ECTS1/2][PBR][MAPPA1/2]
- creazione processo PADRE_TRENI come figlio processo main
- PADRE_TRENI crea i file rappresentanti i segmenti di binario, imposta correttamente i permessi a 666
- PADRE_TRENI fa anche le fork per generare i treni figli
- aggiunto file registro.c che crea distribuisce itinerari ai treni tramite quando ne fanno richiesta, sempre tramite pipe
- processi treni lanciati tramite exec dell'eseguibile processo_treno

//TODO:

1- risistemare header, magari farne più di uno differenziando gli include in base ai processi


/

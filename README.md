# labSO-progetto
Progetto per la parte di laboratorio del corso di Sistemi Operativi A.A. 2021-2022

// 16.05.2022
Adesso è disponibile un makefile funzionante. Opzioni per la compilazione:
- "make" per compilare i tre file .c, dopodichè lanciare il programma movementAuthority. Tutti i file creati durante l'esecuzione, come pipe e file binari, saranno creati nella current directory, rendendo il tutto abbastanza disordinato. Usare "make clean" per ripristinare la situazione iniziale
- [NOPE] NON USARE!!!! "make install" per compilare i tre file .c e organizzare la directory in modo che gli eseguibili vengano inseriti nella cartella bin, i file .c nella cartella source e si possano inoltre creare le cartelle nelle quali organizzare i file. 

// 20.05.2022
Tutti i treni ricevono correttamente gli itinerari e li salvano in una propria struttura dati. 
Una volta ricevuti gli itinerari si mettono in attesa che anche gli altri treni siano pronti e chiamano su sè stessi una SIGSTOP. Sarà il processo registro che, una volta terminato, lancerà una SIGCONT a tutti i processi del suo gruppo di appartenenza, facendo ripartire i processi treno.
Da questo punto basta implementare il movimento dei treni e tutto dovrebbe continuare a funzionare correttamente se questo è implementato correttamente.

// 21.05.2022
Implementato funzionamento in modalità ECTS1.

Manca da aggiungere la produzione dei file di log e risolvere il fatto che alla terminazione di uno dei processi figlio termina anche il processo padre.
Per ora il problema della terminazione del padre è risolto in modo dumb-ino

// 22.05.2022
Implementato management file di log dei treni

Implementata socket lato server e inizio lavoro a comunicazione registro-server

// 24.05.2022

// 25.05.2022

// 26.05.2022



Cose da far notare nella relazione:
- codice in inglese con alcune keyword del testo del progetto mantenute in italiano
- in ETCS2 si possono lanciare server e processo normale in qualsiasi ordine dato che padre treni aspetta il pid del server e il server aspetta registro
- far notare che con registro si comunica solo tramite pipe: la pipe per ricevere richieste dai treni viene ricreata ad ogni iterazione, le pipe usate da registro per mandare gli itinerari ad ogni treno sono invece create da movementAuthority e mai distrutte

Criticità: 
- riga 163 in registro.c, funzione waitForRequest. Se processo treno non avesse già  fatto la richiesta? ritornerebbe falso?
- si possono mettere in header la funzione receiveStage di processo_treno e la funzione waitForRequest di registro per ridurre duplicazione di codice e chiamare la funzione receiveFromPipe()

TODO: 
-mettere in header funzione ricezione da pipe o socket, evitare duplicazione di codice il più possibile
-controllo profondo
-sistemare header, magari dividerlo in più di uno
-preparare makefile install
-problema deadlock mi sa che si è risolto da solo per come abbiamo implementato il tutto, se ne riparla...

//DONE:
- scansione argomenti input e controllo formato [ECTS1/2][PBR][MAPPA1/2]
- creazione processo PADRE_TRENI come figlio processo main
- PADRE_TRENI crea i file rappresentanti i segmenti di binario, imposta correttamente i permessi a 666
- PADRE_TRENI fa anche le fork per generare i treni figli
- aggiunto file registro.c che crea distribuisce itinerari ai treni tramite quando ne fanno richiesta, sempre tramite pipe
- processi treni lanciati tramite exec dell'eseguibile processo_treno

//TODO:

1- risistemare header, magari farne più di uno differenziando gli include in base ai processi


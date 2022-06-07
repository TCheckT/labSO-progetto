# labSO-progetto
Progetto per la parte di laboratorio del corso di Sistemi Operativi A.A. 2021-2022

Per installare, utilizzare il comando "make install". 
Questo creerà la cartella "progetto" con le sottocartelle "bin","log","src".

bin conterrà i file eseguibili del progetto.

log è la cartella dove verranno salvati i file di log dei processi che rappresentano i treni ed eventualmente del server.

src conterrà i codici sorgente degli applicativi.

Per eseguire il programma, spostarsi nella cartella bin e lanciare l'applicativo "movementAuthority" specificando l'opzione etcs, scegliendo tra "ETCS1" o "ETCS2".

Se si vuole lanciare l'applicativo in modalità "ETCS2" è necessario prima lanciare da un altro terminale il server attraverso il comando "./movementAuthority ETCS2 RBC"

In ogni caso, è necessario specificare dopo il lancio di ogni comando anche la mappa su cui si vuole che l'applicativo venga eseguito, specificando "MAPPA1" o "MAPPA2".
Se si lancia il programa in modalità ETCS2 dovrà essere specificata la stessa mappa ad entrambi i comandi.

Istruzioni del makefile:
	
	install:
		Crea la struttura in cartelle del progetto, sposta nella cartella "src" tutti i file sorgente e genera gli eseguibili (cartella "bin").

	update: 
		Utilizzato in fase di implementazione e test del progetto: in caso di modifica di un file sorgente, il comado "make update" rimuove tutti gli eseguibili e ne genera le eventuali nuove versioni eseguendo nuovamente tutti i file sorgente
	
	clean:
		In caso si sia voluto eseguire degli applicativi senza lanciare il comando "make install" (ad esempio per testare un singlolo file) il comando "make clean" rimuoverà dalla cartella tutti gli eseguibili eventualmente creati e i file generati dall'esecuzione stessa, inclusi i file di log, lasciando nella cartella solo i file sorgente

	restore:
		Dopo aver eseguito l'installazione si può lanciare questo comando per rimuovere la struttura ad albero delle cartelle del progetto, lasciando solo i file sorgente. I file eseguibili e quelli creati durante un eventuale eseuzione vengono eliminati nel processo, inculusi i file di log
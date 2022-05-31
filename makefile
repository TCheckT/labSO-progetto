movementAuthority: movementAuthority.c padre_treni registro server_RBC turn_manager header.h
	cc movementAuthority.c -o movementAuthority

padre_treni: padre_treni.c processo_treno header.h 
	cc padre_treni.c -o padre_treni

processo_treno: processo_treno.c header.h
	cc processo_treno.c -o processo_treno

registro: registro.c header.h
	cc registro.c -o registro

server_RBC: server_RBC.c header.h
	cc server_RBC.c -o server_RBC

turn_manager: turn_manager.c header.h
	cc turn_manager.c -o turn_manager


install:
	mkdir progetto
	mkdir progetto/src
	mkdir progetto/bin
	mv header.h progetto/src/
	mv *.c progetto/src/
	cc progetto/src/movementAuthority.c -o progetto/bin/movementAuthority
	cc progetto/src/registro.c -o progetto/bin/registro
	cc progetto/src/padre_treni.c -o progetto/bin/padre_treni
	cc progetto/src/processo_treno.c -o progetto/bin/processo_treno
	cc progetto/src/server_RBC.c -o progetto/bin/server_RBC
	cc progetto/src/turn_manager.c -o progetto/bin/turn_manager

clean:
	rm -f *.o
	rm -f *Pipe*
	rm -f *.log
	rm -f T*
	rm -f movementAuthority registro padre_treni processo_treno server_RBC turn_manager
	rm -f MA*
	rm -f itineraryRequestPipe
	rm -f authorization
	rm -f trackFilesGuard

# to remove: this option must not be present in definitive version of the project
restore:
	mv progetto/src/* .
	rm -rdf progetto

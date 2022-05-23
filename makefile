movementAuthority: movementAuthority.o padre_treni registro server_RBC
	cc movementAuthority.o -o movementAuthority

movementAuthority.o: movementAuthority.c header.h
	cc -c movementAuthority.c

padre_treni: padre_treni.o processo_treno
	cc padre_treni.o -o padre_treni

padre_treni.o: padre_treni.c header.h
	cc -c padre_treni.c

registro: registro.o
	cc registro.o -o registro

registro.o: registro.c header.h
	cc -c registro.c

processo_treno: processo_treno.o
	cc processo_treno.o -o processo_treno

processo_treno.o: processo_treno.c header.h
	cc -c processo_treno.c

server_RBC: server_RBC.o
	cc server_RBC.o -o server_RBC
	
server_RBC.o: server_RBC.c header.h
	cc -c server_RBC.c

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

clean:
	rm -f *.o 
	rm -f *Pipe*
	rm -f *.log
	rm -f T*
	rm -f movementAuthority registro padre_treni processo_treno server_RBC
	rm -f MA*
	rm -f itineraryRequestPipe
	rm -f authorization

# to remove: this option must not be present in definitive version of the project
restore:
	mv progetto/src/* .
	rm -rdf progetto

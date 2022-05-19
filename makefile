movementAuthority: movementAuthority.o padre_treni registro
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

clean:
	rm -f *.o 
	rm -f T*
	rm -f movementAuthority registro padre_treni processo_treno
	rm -f MA*
	rm -f itineraryRequestPipe

# to remove: this option must not be present in definitive version of the project
restore:
	mv progetto/src/* .
	rm -rd progetto

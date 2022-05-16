movementAuthority: movementAuthority.o 
	cc movementAuthority.o -o movementAuthority

movementAuthority.o: movementAuthority.c header.h
	cc -c movementAuthority.c

padre_treni: padre_treni.o
	cc padre_treni.o 

padre_treni.o: padre_treni.c header.h
	cc -c padre_treni.c
	
install: 
	mkdir progetto
	mkdir progetto/include
	mkdir progetto/src
	mkdir progetto/bin
	mv header.h progetto/include/
	mv *.c progetto/src/
	cc progetto/src/movementAuthority.c -o progetto/bin/movementAuthority
	cc progetto/src/registro.c -o progetto/bin/registro
	cc progetto/src/padre_treni.c -o progetto/bin/padre_treni

clean:
	rm *.o
	rm -rd progetto	
	rm MA*

# to remove: this option must not be present in definitive version of the project
restore:
	mv progetto/include/* .
	mv progetto/src/* .
	rm -rd progetto

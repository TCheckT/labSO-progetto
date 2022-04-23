movementAuthority: movementAuthority.o 
	cc movementAuthority.o -o movementAuthority

movementAuthority.o: movementAuthority.c header.h
	cc -c movementAuthority.c

padre_treni: padre_treni.o
	cc padre_treni.o -o padre_treni

padre_treni.o: padre_treni.c header.h
	cc -c padre_treni.c
	
install: 
	mkdir progetto
	mkdir progetto/include
	mkdir progetto/src
	mkdir progetto/bin
	mv movementAuthority progetto/bin
	mv padre_treni progetto/bin
	cp movementAuthority.c progetto/src
	cp padre_treni.c progetto/src
	cp header.h progetto/include
	cp movementAuthority.make progetto

clean:
	rm *.o
	rm -r progetto	
	rm MA*

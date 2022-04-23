main: main.o 
	cc main.o -o main

main.o: main.c header.h
	cc -c main.c

padre_treni: padre_treni.o
	cc padre_treni.o -o padre_treni

padre_treni.o: padre_treni.c header.h
	cc -c padre_treni.c
	
install: 
	mkdir progetto
	mkdir progetto/include
	mkdir progetto/src
	mkdir progetto/bin
	mv main progetto/bin
	mv padre_treni progetto/bin
	cp main.c progetto/src
	cp padre_treni.c progetto/src
	cp header.h progetto/include
	cp main.make progetto

clean:
	rm *.o
	rm -r progetto	
	rm MA*

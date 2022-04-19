main: main.o
	cc main.o -o main

main.o: main.c header.h
	cc -c main.c
	
install: 
	mkdir progetto
	mkdir progetto/include
	mkdir progetto/src
	mkdir progetto/bin
	mv main progetto/bin
	cp main.c progetto/src
	cp header.h progetto/include
	cp main.make progetto

clean:
	rm *.o
	rm -r progetto	
	rm MA*

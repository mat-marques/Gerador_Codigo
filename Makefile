CFLAGS = -g -std=c++11


gerador: gerador.o Arquivo.o
	g++ $(CFLAGS) -o gerador gerador.o Arquivo.o

gerador.o: gerador.cpp
	g++ $(CFLAGS) -c gerador.cpp -o gerador.o

Arquivo.o: Arquivo.h Arquivo.cpp
	g++ $(CFLAGS) -c Arquivo.cpp -o Arquivo.o

.PHONY:
	clean

clean:
	rm -f *.o
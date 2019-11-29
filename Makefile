CFLAGS = -g -std=c++11
gerador: gerador.o Arquivo.o AST.o HashTable.o Item.o
	g++ $(CFLAGS) gerador.o Arquivo.o AST.o HashTable.o Item.o -o gerador
	
Arquivo.o: Arquivo.h Arquivo.cpp
	g++ $(CFLAGS) -c Arquivo.cpp -o Arquivo.o

HashTable.o: HashTable.h HashTable.cpp Item.h Item.cpp
	g++ $(CFLAGS) -c HashTable.cpp -o HashTable.o

Item.o: Item.h Item.cpp
	g++ $(CFLAGS) -c Item.cpp -o Item.o

AST.o: AST.h AST.cpp HashTable.h HashTable.cpp
	g++ $(CFLAGS) -c AST.cpp -o AST.o

gerador.o: gerador.cpp Arquivo.h Arquivo.cpp AST.h AST.cpp
	g++ $(CFLAGS) -c gerador.cpp -o gerador.o

.PHONY:
	clean

clean:
	rm -f *.o
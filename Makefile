CFLAGS = -g -std=c++11 -Wall
cc = -Wall -g -std=c++11
gerador: gerador.o Arquivo.o AST.o HashTable.o Item.o	graph.o	vertice.o	graphColoring.o	liveness.o	regalloc.o
	g++ $(CFLAGS) gerador.o Arquivo.o AST.o regalloc.o liveness.o HashTable.o Item.o graph.o	vertice.o	graphColoring.o -o gerador
	
Arquivo.o: Arquivo.h Arquivo.cpp
	g++ $(CFLAGS) -c Arquivo.cpp -o Arquivo.o

HashTable.o: HashTable.h HashTable.cpp Item.h Item.cpp
	g++ $(CFLAGS) -c HashTable.cpp -o HashTable.o

Item.o: Item.h Item.cpp
	g++ $(CFLAGS) -c Item.cpp -o Item.o

AST.o: AST.h AST.cpp HashTable.h HashTable.cpp
	g++ $(CFLAGS) -c AST.cpp -o AST.o

regalloc.o:	regalloc.cpp	regalloc.h	liveness.h	graph.h	graphColoring.h AST.h
	g++	$(cc)	-c regalloc.cpp

gerador.o: gerador.cpp Arquivo.h Arquivo.cpp AST.h AST.cpp regalloc.h regalloc.cpp
	g++ $(CFLAGS) -c gerador.cpp -o gerador.o

graph.o:	graph.h	graph.cc vertice.h
	g++ $(cc) -c graph.cc

vertice.o:	vertice.cc vertice.h
	g++ $(cc) -c vertice.cc

graphColoring.o:	graphColoring.cc graphColoring.h graph.h
	g++ $(cc) -c graphColoring.cc

liveness.o:	liveness.cc	liveness.h	graph.h	AST.h
	g++	$(cc) -c liveness.cc

.PHONY:
	clean

clean:
	rm -f *.o
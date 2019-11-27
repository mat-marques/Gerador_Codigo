CFLAGS = -g -std=c++11


gerador: gerador.o HashTable.o List.o Item.o AST.o
	g++ $(CFLAGS) -o gerador gerador.o HashTable.o List.o Item.o AST.o

gerador.o: AST.h AST.cpp gerador.cpp
	g++ $(CFLAGS) -c gerador.cpp -o gerador.o

HashTable.o: HashTable.h HashTable.cpp List.h List.cpp
	g++ $(CFLAGS) -c HashTable.cpp -o HashTable.o

List.o: List.h List.cpp Item.h Item.cpp
	gcc $(CFLAGS) -c List.cpp -o List.o

Item.o: Item.h Item.cpp
	g++ $(CFLAGS) -c Item.cpp -o Item.o

AST.o: AST.h AST.cpp HashTable.h HashTable.cpp
	g++ $(CFLAGS) -c AST.cpp -o AST.o

.PHONY:
	clean

clean:
	rm -f *.o
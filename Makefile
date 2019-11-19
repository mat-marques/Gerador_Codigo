CFLAGS = -g -std=c++11
build = ./build/
lexico = ./lexico/
sintatico = ./sintatico/


main: $(build)main.o $(build)HashTable.o $(build)List.o $(build)Item.o $(build)Lexicon.o $(build)Parser.o $(build)Token.o $(build)AST.o
	g++ $(CFLAGS) -o main $(build)main.o $(build)HashTable.o $(build)List.o $(build)Item.o $(build)Lexicon.o $(build)Parser.o $(build)Token.o $(build)AST.o

$(build)main.o: $(sintatico)Parser.h $(sintatico)Parser.cpp main.cpp
	g++ $(CFLAGS) -c main.cpp -o $(build)main.o

$(build)HashTable.o: $(lexico)HashTable.h $(lexico)HashTable.cpp $(lexico)List.h $(lexico)List.cpp
	g++ $(CFLAGS) -c $(lexico)HashTable.cpp -o $(build)HashTable.o

$(build)List.o: $(lexico)List.h $(lexico)List.cpp $(lexico)Item.h $(lexico)Item.cpp
	gcc $(CFLAGS) -c $(lexico)List.cpp -o $(build)List.o

$(build)Item.o: $(lexico)Item.h $(lexico)Item.cpp
	g++ $(CFLAGS) -c $(lexico)Item.cpp -o $(build)Item.o

$(build)Token.o: $(lexico)Token.h $(lexico)Token.cpp
	g++ $(CFLAGS) -c $(lexico)Token.cpp -o $(build)Token.o

$(build)Lexicon.o: $(lexico)Lexicon.h $(lexico)Lexicon.cpp $(lexico)HashTable.h $(lexico)HashTable.cpp $(lexico)Token.h $(lexico)Token.cpp
	g++ $(CFLAGS) -c $(lexico)Lexicon.cpp -o $(build)Lexicon.o

$(build)AST.o: $(sintatico)AST.h $(sintatico)AST.cpp $(lexico)HashTable.h $(lexico)HashTable.cpp
	g++ $(CFLAGS) -c $(sintatico)AST.cpp -o $(build)AST.o

$(build)Parser.o: $(sintatico)Parser.h $(sintatico)Parser.cpp $(lexico)Lexicon.h $(lexico)Lexicon.cpp $(sintatico)AST.h $(sintatico)AST.cpp 
	g++ $(CFLAGS) -c $(sintatico)Parser.cpp -o $(build)Parser.o

.PHONY:
	clean

clean:
	rm edit $(build)*.o
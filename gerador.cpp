#include <iostream>
#include <string>
#include "Arquivo.h"
#include "AST.h"

using namespace std;

int main() {
	ofstream mipsFile;
    mipsFile.open ("MipsIntermediario.txt");
   string input;
   vector<string> file_input;
   //Leitura da entrada
//   while(getline(cin, input)) {
//      file_input.push_back(input);
//   }
   Program *p;
   file_input.push_back("WHILE(&&(!=(+(c,2),2),!=(+(c,2),2)),=(c,a);RETURN(0));");
   file_input.push_back("FOR(=(i,1),<(i,10),=(i,+(i,1)),RETURN(0))");
   //file_input.push_back("DO_WHILE(=(c,a);RETURN(0),!=(+(c,2),2))");
   Arquivo *file = new Arquivo();
   p = file->readFile(file_input);

   p->mipsProgram(mipsFile);

   p->mips();
   mipsFile.close();

   return 0;

}


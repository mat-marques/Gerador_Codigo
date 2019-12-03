#ifndef _ARQUIVO_
#define _ARQUIVO_

#include <iostream>
#include <vector>
#include <string.h>
#include "AST.h"

using namespace std;

class Arquivo{
    public:
        Program *readFile(vector<string> file_input);
        Arquivo(){};
};

#endif

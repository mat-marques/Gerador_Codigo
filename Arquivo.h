#ifndef _ARQUIVO_
#define _ARQUIVO_

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

class Arquivo{
    public:
        void readFile(vector<string> file_input);
        Arquivo(vector<string> file_input);
};

#endif
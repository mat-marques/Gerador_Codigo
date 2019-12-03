#ifndef _GRAPH_
#define _GRAPH_

#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<iterator>
#include"vertice.h"

class graph
{
    private: 
        int id;
        int K;
        int regFisicos;
        std::list<vertice*> vertices;

    public:
        graph(int id1, int K1);
        ~graph();
        void addVertice(int id1, int K1, int regFisicos);
        void insertVertice(vertice *newVertice);
        void showIdAresta();
        void addInterferencias();
        int getRegFisicos();
        int getK();
        int getId();
        void setK(int K);
        std::list<vertice*> simplify(int K1);
        vertice* potencialSpill(int K1);
        void resetGraph(int K);
};

#endif
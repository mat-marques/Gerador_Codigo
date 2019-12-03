#ifndef _VERTICE_
#define _VERTICE_

#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<iterator>
#include<iostream>
#include<string>

class vertice
{
    private:
    std::string id;
    bool *cores;
    bool regFisico;
    int cor;
    bool ativo;
    int arestaQtd;
    std::list<std::string> arestas;
    std::list<vertice*> interferencias;
    
    public:
        vertice(int id1, int K1);
        ~vertice();
        void addAresta(std::string id1);
        void setCor(bool color,int idcor);
        std::string getId();
        void showIdAresta();
        void resetVertice(int K);
        void addInterferencia(vertice *newVertice);
        std::list<std::string> getArestas();
        int sizeOfInterferencia();
        bool isActive();
        void setArestaQtd(int Qtd);
        int getArestaQtd();
        void setDisable();
        bool putColor(int K1);
        int getCor();
};

#endif

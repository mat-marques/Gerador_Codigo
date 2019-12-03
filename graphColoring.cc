#include"graphColoring.h"
#include <string>

void coloringGraph(graph *grafo)
{
    int K;
    std::list<bool> resultado;
    std::cout << "Graph " <<grafo->getId() << " -> Physical Registers: " << grafo->getRegFisicos() << "\n" << "----------------------------------------\n----------------------------------------\n";
    K = grafo->getK();
    while(K > 1)
    {
        std::list<vertice*> pilha;
        std::list<graph*>::iterator node;
        std::cout << "K = " << K << "\n";
        pilha = grafo->simplify(K);
        resultado.push_back(coloring(pilha, K));
        K--;
        std::cout << "----------------------------------------\n";
        grafo->resetGraph(K);
        pilha.clear();
    }
    std::cout << "----------------------------------------";
    printResultado(resultado, grafo->getId(), grafo->getK());
    resultado.clear();
}

bool coloring(std::list<vertice*> pilha, int K)
{
    std::list<vertice*>::iterator node;
    vertice* aux;
    bool succesfull;
    std::cout << "\n";
    for(node = pilha.begin(); node != pilha.end(); node++)
    {
        aux = *node;
        succesfull = aux->putColor(K);
        if(succesfull == false)
        {
            std::cout << "Pop: " << aux->getId() << " -> NO COLOR AVAILABLE\n";
            return false;
        }
        else
        {
            std::cout << "Pop: " << aux->getId() << " -> " << aux->getCor() << "\n";
        }
    }
    return true;
}

void printResultado(std::list<bool> resultado, int idGraph, int K)
{
    std::list<bool>::iterator node;
    bool moreThanTen;
    if(K >= 10)
    {
        moreThanTen = true;
    }

    for(node = resultado.begin(); node != resultado.end(); node++)
    {
        std::cout << "\n";
        if(moreThanTen && K < 10)
        {
            std::cout << "Graph " << idGraph << " -> K =  " << K << ": ";    
        }
        else
        {
            std::cout << "Graph " << idGraph << " -> K = " << K << ": ";
        }

        if(*node)
        {
            std::cout << "Successful Allocation";   
        }
        else
        {
            std::cout << "SPILL";
        }
        K--;
    }
}
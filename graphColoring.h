#ifndef _GRAPH_COLORING_
#define _GRAPH_COLORING_

#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<iterator>
#include"graph.h"

void coloringGraph(graph *grafo);
bool coloring(std::list<vertice*> pilha, int K);
void printResultado(std::list<bool> resultado, int idGraph, int K);

#endif
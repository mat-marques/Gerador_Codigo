#include"graph.h"

graph::graph(int id1, int K1)
{
        id = id1;
        regFisicos = K1;
        K = K1;  
}

graph::~graph() 
{
    if(!(vertices.empty()))
    {
        std::list<vertice*>:: iterator node;
        vertice* aux;
        for(node = vertices.begin(); node != vertices.end(); node++)
        {
            aux = *node;
            delete(aux);
        }
    }
}

void graph::addVertice(int id1, int K1, int regFisicos)
{
    vertices.push_front(new vertice(id1, K1));
}

void graph::insertVertice(vertice *newVertice)
{
    vertices.push_front(newVertice);
}

void graph::showIdAresta()
{
    vertice *aux;
    std::list<vertice*> :: iterator node;
    for(node = vertices.begin(); node != vertices.end(); node++)
    {
        aux = *node;
        aux->showIdAresta();
        std::cout << '\n';
    }
}

void graph::addInterferencias()
{
    std::list<std::string> arestas;
    std::list<std::string>::iterator aresta;
    std::list<vertice*>:: iterator node, node2;
    vertice *aux, *aux2;
    for(node = vertices.begin(); node != vertices.end(); node++)
    {
        aux = *node;
        arestas = aux->getArestas();
            for(aresta = arestas.begin(); aresta != arestas.end(); aresta++)
            {
                for(node2 = vertices.begin(); node2 != vertices.end(); node2++)
                {
                    aux2 = *node2;
                    if(*aresta == aux2->getId())
                    {
                       aux->addInterferencia(aux2);
                    }
                }
            }
        aux->setArestaQtd(aux->sizeOfInterferencia());
    }  
}

int graph::getRegFisicos()
{
    return regFisicos;
}

int graph::getK()
{
    return K;
}

int graph::getId()
{
    return id;
}

void graph::setK(int K1)
{
    K = K1;
}

std::list<vertice*> graph::simplify(int K1)
{
    int menorK,menorId;
    std::list<vertice*> pilha;
    std::list<vertice*>::iterator node;
    vertice *menor,*aux;
    while(pilha.size() < (vertices.size()-regFisicos))
    {
        std::cout << "\n";
        menorK = -1;
        for(node = vertices.begin(); node != vertices.end(); node++)
        {
            aux = *node;
            if(aux->isActive() == true)
            {
                if(aux->getArestaQtd() < K1)
                {
                    if(menorK < 0)
                    {
                        menor = aux;
                        menorK = aux->getArestaQtd();
                    }
                    else if(aux->getArestaQtd() < menorK)
                    {
                        menor = aux;
                        menorK = aux->getArestaQtd();
                    }
                }
            }
        }
        if(menorK < 0)
        {
            pilha.push_front(potencialSpill(K));
        }
        else
        {
            pilha.push_front(menor);
            menor->setDisable();
            std::cout << "Push: " << menor->getId();
        }
    }
    return pilha;
}

vertice* graph::potencialSpill(int K1)
{
    int maiorK,maiorId;
    std::list<vertice*>::iterator node;
    vertice *maior,*aux;
    maiorK = -1;
    for(node = vertices.begin(); node != vertices.end(); node++)
    {
        aux = *node;
        if(aux->isActive() == true)
        {
            if(aux->getArestaQtd() > maiorK)
            {
                maior = aux;
                maiorK = aux->getArestaQtd();
            }
        }
    }   
    std::cout << "Push: " << maior->getId() << " *";
    maior->setDisable();
    return maior;
}

void graph::resetGraph(int K)
{
    std::list<std::string> arestas;
    std::list<std::string>::iterator aresta;
    std::list<vertice*>::iterator node;
    vertice* aux;
    for(node = vertices.begin(); node != vertices.end(); node++)
    {
        aux = *node;
        aux->resetVertice(K);
        arestas = aux->getArestas();
        aux->setArestaQtd(aux->sizeOfInterferencia());
    }
}
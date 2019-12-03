#include"vertice.h"


vertice::vertice(int id1, int K1)
{
    int i;
    regFisico = false;
    id = id1;
    ativo = true;
    cores = (bool *) malloc(sizeof(bool)*K1);
    for(i = 0; i < K1; i++)
    {
        cores[i] = false;
    }
}

vertice::~vertice()
{
    if(cores != NULL)
    {
        free(cores);
    }
}

void vertice::addAresta(std::string id1)
{
    arestas.push_front(id);
}

void vertice::setCor(bool color, int idcor)
{
    cores[idcor] = color;
}

std::string vertice::getId()
{
    return id;
}

void vertice::showIdAresta()
{
    std::list<std::string> :: iterator node;
    for(node = arestas.begin(); node != arestas.end(); node++)
    {
        std::cout << *node << '\n';
    }
}

void vertice::resetVertice(int K)
{
    int i;
    if(!regFisico)
    {
        for(i = 0; i < K; i++)
        {
            cores[i] = false;
        }
    }
    ativo = true;
}

void vertice::addInterferencia(vertice *newVertice)
{
    interferencias.push_back(newVertice);
}

std::list<std::string> vertice::getArestas()
{
    return arestas;
}

int vertice::sizeOfInterferencia()
{
    return arestas.size();
}

bool vertice::isActive()
{
    return ativo;
}

void vertice::setArestaQtd(int Qtd)
{
    arestaQtd = Qtd;
}

int vertice::getArestaQtd()
{
    return arestaQtd;
}

void vertice::setDisable()
{
    std::list<vertice*>::iterator node;
    vertice* aux;
    ativo = false;
    for(node = interferencias.begin(); node != interferencias.end(); node++)
    {
        aux = *node;
        aux->setArestaQtd(aux->getArestaQtd()-1);
    }
}

bool vertice::putColor(int K1)
{
    int i;
    std::list<vertice*>::iterator node;
    vertice* aux;
    for(i= 0; i < K1; i++)
    {
        if(!cores[i])
        {
            cor = i;
            cores[i] = true;
            for(node = interferencias.begin(); node != interferencias.end(); node++)
            {
                aux = *node;
                aux->setCor(true, i);
            }
            return true;
        }
    }
    return false;
}

int vertice::getCor()
{
    return cor;
}
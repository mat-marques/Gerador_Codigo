#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// A classe Item poder ser os seguintes itens:
// variáveis: nome, tipo, dimensão, se são locais ou não, se são parâmetros de função.
// parâmetros de função.
// funções: nome, parâmetros, variáveis locais, tipo de retorno.
// protótipos: nome, parâmetros, tipo de retorno.
// constantes: nome, valores.
class Item {
  public:
    string var_category = ""; //Categoria do item: variável, função, protótipos, constantes e parâmetro

    //variável, função, protótipos, constantes e parâmetro
    string var_name = ""; //Nome do item

    //Variáveis e parâmetro
    string var_type = ""; //Categoria do item: int, float, ...
    vector<int> var_dimension; //Dimensão da variavel, caso for um vetor

    //Funções e Parâmetros
    string var_params = ""; //Parâmetros item
    string var_return = ""; //Retorno da função ou parâmetro
    
    //Constantes
    string var_value = ""; //Valor da constante

    Item();
    void ItemVariable(string var_category, string var_name, string var_type, vector<int> var_dimension);
    void ItemParam(string var_category, string var_name, string var_type, vector<int> var_dimension);
    void ItemConst(string var_category, string var_name, string var_value);
    void ItemFunction(string var_category, string var_name, string var_type, string var_return);
    void ItemPrototypes(string var_category, string var_name, string var_type);
    string toString();
};

#endif

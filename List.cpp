#include <iostream>
#include <string>
#include "List.h"

/*
  Construtor da classe.
*/
List::List(){
  this->head = NULL;
  this->tail = NULL;
  this->size = 0;
}

/*
  Adiciona no inicio da lista.
*/
bool List::addStart(Item *data){
  Node *temp = new Node;
  temp->data = data;
  if(this->size > 0){
    temp->next = this->head;
    this->head = temp;
  } else {
    this->head = temp;
    this->tail = temp;
  }
  this->size = this->size + 1;
  return true;
}

/*
  Adiciona no final da lista.
*/
bool List::addFinal(Item *data){
  Node *temp = new Node;
  temp->data = data;
  temp->next = NULL;
  
  if(this->size > 0){
    this->tail->next = temp;
    this->tail = temp;
  } else {
    this->head = temp;
    this->tail = temp;
  }
  this->size = this->size + 1;
  return true;
}

/*
  Remove um elemento da lista.
*/
Item * List::remove(Item data){
  int i;
  Node *temp, *aux;
  if(this->size > 0){
    temp = this->head;
    for(i = 0; i < this->size; i++){
      if(data.var_name.compare(temp->data->var_name) == 0) {

        if(i == 0){
          this->head = this->head->next;
          Item *data = temp->data;
          delete temp;
          return data;
        } else if (i == this->size - 1) {
          this->tail = aux;
          Item *data = temp->data;
          delete temp;
          return data;
        } else {
          aux->next = temp->next;
          Item *data = temp->data;
          delete temp;
          return data;
        }

      }
      aux = temp;
      temp = temp->next;
    }
    this->size = this->size - 1;
  }
}

/*
  Pesquisa um elemento na lista.
*/
Item * List::search(Item data){
  int i;
  if(this->size > 0){
    Node *temp = this->head;
    for(i = 0; i < this->size; i++){
      if(data.var_name.compare(temp->data->var_name) == 0 ) {
        return temp->data;
      }
      temp = temp->next;
    }
  }
  return NULL;
}


/*
  Pesquisa um elemento na lista pela posição.
*/
Item * List::searchByIndex(int index){
  int i;
  if(this->size > 0){
    Node *temp = this->head;
    for(i = 0; i < this->size; i++){
      if(i == index) {
        return temp->data;
      }
      temp = temp->next;
    }
  }
  return NULL;
}

/*
  Escreve os dados em um arquivo.
*/
void List::showItens(ofstream & hFile, string nameList){
    Node *temp;
    temp = this->head;
    for(int i =0; i < this->size; i++){
        hFile << nameList << ", Posicao: " << (i + 1) << ", Palavra: "<< temp->data->var_name << "\n";
        temp = temp->next;
    }
}

void List::showItens2(string nameFile){
    Node *temp;
    ofstream hFile;
    hFile.open (nameFile);
    temp = this->head;
    for(int i =0; i < this->size; i++){
        hFile << temp->data->var_type << " " << temp->data->var_name << endl;
        temp = temp->next;
    }
    hFile.close();
}


/*
  Apaga a lista.
*/
void List::removeList(){
  int i;
  Node *temp, *aux;
  temp = this->head;
  for(i = 0; i < this->size; i++){
    delete temp->data;
    aux = temp;
    temp = temp->next;
    delete aux;
  }
  this->size = 0;
  this->head = NULL;
  this->tail = NULL;
}

/*
  Retorna o tamanho da lista.
*/
int List::getSize(){
    return this->size;
}

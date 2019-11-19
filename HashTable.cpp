#include "HashTable.h"

/*
  Construtor da classe.
*/
HashTable::HashTable(int size){
  this->hashtable = new List*[size];
  for(int i = 0; i < size; i++){
    this->hashtable[i] = new List();
  }
  this->size = size;
}

/*
  insere um elemento na hashTable.
*/
bool HashTable::insertItem(Item item){
  List *l;
  int hashKey = this->hashFunction(item.var_name);
  l = this->hashtable[hashKey];
  l->addFinal(new Item(item.var_name, item.var_category, item.var_level, item.var_type, item.var_displacement, item.var_reference));
  return false;
}

/*
  Pesquisa um item na hashTable. Retorna o item.
*/
Item *HashTable::searchItem(string item){
  List *l;
  int hashKey = this->hashFunction(item);
  l = this->hashtable[hashKey];
  Item *it = l->search(Item(item));
  return it;
}

/*
  Pesquisa um item na hashTable. Retorna um booleano.
*/
bool HashTable::searchItem2(string item){
  List *l;
  int hashKey = this->hashFunction(item);
  l = this->hashtable[hashKey];
  Item *it = l->search(Item(item));

  if(it != NULL){
    return true;
  }

  return false;
}

/*
  Pesquisa um item na hashTable. Retorna o item.
*/
Item *HashTable::searchItem3(int hashIndex, int wordIndex){
  List *l;
  l = this->hashtable[hashIndex];
  Item *it = l->searchByIndex(wordIndex);
  return it;
}

/*
  Escreve os itens da lista.
*/
void HashTable::show(string outFile){
  int i =0;
  ofstream hFile;
  hFile.open (outFile);

  while(i < this->size){
    List *l = this->hashtable[i];
    if(l->getSize() > 0) {
      string ln = "Lista " + std::to_string(i);
      l->showItens(hFile, ln);
    }
    i++;
  }

  hFile.close();
}

/*
  Escreve os itens da lista.
*/
void HashTable::show2(string outFile){
  int i =0;
  ofstream hFile;
  hFile.open (outFile);

  while(i < this->size){
    List *l = this->hashtable[i];
    if(l->getSize() > 0) {
      string ln = std::to_string(i);
      l->showItens3(hFile, ln);
    }
    i++;
  }

  hFile.close();
}

/*
  Deleta um item da hashTable.
*/
Item *HashTable::deleteItem(Item item){
  List *l;
  int hashKey = this->hashFunction(item.var_name);
  l = this->hashtable[hashKey];
  Item *it = l->remove(Item(item.var_name));
  return it;
}

/*
  Função que calcula a posição que a string será inserida no vetor.
*/
int HashTable::hashFunction(string item){
  unsigned long int h = 0, alfa = 10;
  for(unsigned long int i=0; i<item.length(); i++){
    h = (alfa * h) + item[i];
  }
  h = h % this->size;
  return h;
}

/*
  Apaga a hashTable.
*/
void HashTable::removeHashTable(){
  int i =0;
  while(i < this->size){
    List *l = this->hashtable[i];
    l->removeList();
    delete l;
    this->hashtable[i] = NULL;
    i++;
  }
  delete[] this->hashtable;
}

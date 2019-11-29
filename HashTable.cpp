#include "HashTable.h"

/*
  Construtor da classe.
*/
HashTable::HashTable(int size){
  this->hashtable.resize(size);
  this->size = size;
}

/*
  insere um elemento na hashTable.
*/
bool HashTable::insertItem(Item *item){
  HashItem *l;
  int hashKey = this->hashFunction(item->var_name);
  l = this->hashtable[hashKey];
  l->itemList.push_back(item);
  return true;
}

int HashTable::search(HashItem hash_item, string itemName) {
  for(int i = 0; i < hash_item.itemList.size(); i++){
    if(hash_item.itemList[i]->var_name == itemName) {
      return i;
    }
  }
  return -1;
}

/*
  Pesquisa um item na hashTable. Retorna o item.
*/
Item *HashTable::searchItem(string itemName){
  HashItem *l;
  Item *it;
  int i;
  int hashKey = this->hashFunction(itemName);
  l = this->hashtable[hashKey];
  i = search(*l, itemName);
  if(i != -1)return l->itemList[i];
  else return NULL;
}

/*
  Escreve os itens da lista.
*/
void HashTable::show(string outFile){
  int i =0;
  string result;
  ofstream hFile;
  hFile.open (outFile);
  for(int i = 0; i < this->hashtable.size(); i++){
    HashItem *h = this->hashtable[i];
    for(int j = 0; j < h->itemList.size(); j++){
      result = h->itemList[j]->toString();
      hFile << result;
    }
  }

  hFile.close();
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


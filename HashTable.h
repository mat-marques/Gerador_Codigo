#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>

#include "List.h"

using namespace std;


class HashTable {

  public:
    int size;
    List **hashtable;
    HashTable(int size);
    bool insertItem(Item item);
    Item *deleteItem(Item item);
    Item *searchItem(string item);
    bool searchItem2(string item);
    Item *searchItem3(int hashIndex, int wordIndex);
    void show(string outFile);
    void show2(string outFile);
    void removeHashTable();
    
  private:
    int hashFunction(string item);

};

#endif

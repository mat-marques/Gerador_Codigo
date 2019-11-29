#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>

#include "Item.h"
using namespace std;


class HashItem {
  public:
    vector<Item*> itemList;
};

class HashTable {

  public:
    int size;
    vector<HashItem*> hashtable;
    
    HashTable(int size);
    bool insertItem(Item *item);
    Item *searchItem(string itemName);
    void show(string outFile);
    
  private:
    int hashFunction(string item);
    int search(HashItem hash_item, string itemName);

};

#endif

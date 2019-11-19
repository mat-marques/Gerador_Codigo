#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Item.h"

using namespace std;

struct Node
{
  Item *data;
  struct Node *next;
};	


class List {
    private:
      struct Node *head, *tail;
      int size;
      
    public:
      List();
      bool addStart(Item *data);
      bool addFinal(Item *data);
      Item *remove(Item data);
      Item *search(Item data);
      Item *searchByIndex(int index);
      void showItens(ofstream & hFile, string nameList);
      void showItens3(ofstream & hFile, string nameList);
      void showItens2(string nameFile);
      int getSize();
      void removeList();
};

#endif

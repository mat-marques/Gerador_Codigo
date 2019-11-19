#include <iostream>
#include <string>
#include "Item.h"

Item::Item() {}

void Item::ItemVariable(string var_category, string var_name, string var_type, string var_dimension) {
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_type = var_type;
    this->var_dimension = var_dimension;
}

void Item::ItemParam(string var_category, string var_name, string var_type, string var_dimension){
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_type = var_type;
    this->var_dimension = var_dimension;
}
void Item::ItemConst(string var_category, string var_name, string var_value){
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_value = var_value;
}

void Item::ItemFunction(string var_category, string var_name, string var_type, string var_return){
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_type = var_type;
    this->var_return = var_return;
}

void Item::ItemPrototypes(string var_category, string var_name, string var_type){
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_type = var_type;
}
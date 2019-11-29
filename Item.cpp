#include <iostream>
#include <string>
#include "Item.h"

Item::Item() {}

void Item::ItemVariable(string var_category, string var_name, string var_type, vector<int> var_dimension) {
    this->var_category = var_category;
    this->var_name = var_name;
    this->var_type = var_type;
    this->var_dimension = var_dimension;
}

void Item::ItemParam(string var_category, string var_name, string var_type, vector<int> var_dimension){
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

string Item::toString(){
    string result = "";

    if(this->var_name != ""){
        result = result + this->var_name;
    }

    if(this->var_type != ""){
        result = result + ", " + this->var_name;
    }

    if(this->var_category != ""){
        result = result + ", " + this->var_category;
    }

    if(this->var_params != ""){
        result = result + ", " + this->var_params;
    }

    if(this->var_return != ""){
        result = result + ", " + this->var_return;
    }

    if(this->var_value != ""){
        result = result + ", " + this->var_value;
    }
    if(this->var_dimension.size() >=1){
        result = result + ", ";
        for(int i=0; i < this->var_dimension.size(); i++) {
            result = result + "[" + to_string(this->var_dimension[i]) + "]";
        }
    }

    return result;
}
#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include "HashTable.h"

using namespace std;

class Expressions;
class CondicionalExpression;
class DoWhile;
class While;
class IF;
class For;
class Scanf;
class Printf;
class Exit;
class Return;
class SimpleExpression;
class Expressions;
class Statement;
class Function;
class Program;

class Program {
    public:
        HashTable symbolTable;
        list<Function> functionsList;
};


class Function{
    public:
        HashTable symbolTable;
        list<string> paramsList;
        string return_type;
        list<Statement> statementList;
};

class Statement {
    public:
        list<DoWhile> dowhileList;
        list<While> whileList;
        list<IF> ifList;
        list<For> forList;
        list<Scanf> scanfList;
        list<Printf> printfList;
        list<Exit> exitList;
        list<Return> returnList;
        list<Expressions> expressionsList;
};

class DoWhile {
    public:
        list<Statement> statementList;
        CondicionalExpression condExp;
};

class While {
    public:
        CondicionalExpression condExp;
        list<Statement> statementList;
};

class IF {
    public:
        CondicionalExpression condExp;
        list<Statement> statementListThen;
        list<Statement> statementListElse;
};


class CondicionalExpression {
    public:
        SimpleExpression *lef; 
        string op; 
        SimpleExpression *right;
};


class For {
    public:
        list<string> valuesInitialization;
        CondicionalExpression condExp;
        list<string> valuesAdjustment;
        list<Statement> statementList;
};

class Printf {
    public:
        string out;
        list<Expression> expressionsList;
};

class Scanf {
    public:
        string in;
        string variable;
};

class Exit {
    public:
        Expression exp;
};

class Return {
    public:
        Expression exp;
};

class SimpleExpression {
    public:
        string op;
        string term;
        SimpleExpression *simpleExpressions;
  
};

class Expression {
    public:
        SimpleExpression *left; 
        string op; 
        SimpleExpression *right;
};

#endif
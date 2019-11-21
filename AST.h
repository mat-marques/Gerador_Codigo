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
        string name;
        list<string> paramsList;
        string return_type;
        list<Statement> statementList;
        void mipsFunction(ofstream & mipsFile);
};

class ObjectStatement {
    public:
        void *statementClass;
        string className;
};

class Statement {
    public:
        list<ObjectStatement> statement;
        void mipsStatement(ofstream & mipsFile, int ilabel, string funcName);
};

class DoWhile {
    public:
        list<Statement> statementList;
        BoolOperatorCondicional condExp;
        void mipsDoWhile(ofstream & mipsFile, int label, string funcName);
};

class While {
    public:
        BoolOperatorCondicional condExp;
        list<Statement> statementList;
        void mipsWhile(ofstream & mipsFile, int label, string funcName);
};

class IF {
    public:
        BoolOperatorCondicional condExp;
        list<Statement> statementListThen;
        list<Statement> statementListElse;
        void mipsIF(ofstream & mipsFile, int labelElse, string funcName);
};


class BoolOperatorCondicional {
    public:
        CondicionalExpression *condExpLeft;
        string boolOperator; // && e ||
        CondicionalExpression *condExpRight;
        BoolOperatorCondicional *nextBOC;
        void mipsBOCAnd(ofstream & mipsFile, string op, string R1, string R2, string label);
        void mipsBOCOR(ofstream & mipsFile, string op, string R1, string R2, string label);
        void mipsBOCBoolCondicional(ofstream & mipsFile, string labelThen, string labelElse);
};

class CondicionalExpression {
    public:
        Expression *lef; 
        string op;  // <, >, <=, >=. !=, ==
        Expression *right;
};


class For {
    public:
        list<Expression> valuesInitialization;
        BoolOperatorCondicional condExp;
        list<Expression> valuesAdjustment;
        list<Statement> statementList;
        void mipsFor(ofstream & mipsFile, int label, string funcName);
};

class Printf {
    public:
        string out;
        list<Expression> expressionsList;
        void mipsPrintf(ofstream & mipsFile);
};

class Scanf {
    public:
        string in;
        string variable;
        void mipsScanf(ofstream & mipsFile);
};

class Exit {
    public:
        Expression exp;
        void mipsExit(ofstream & mipsFile);
};

class Return {
    public:
        Expression exp;
        void mipsReturn(ofstream & mipsFile);
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
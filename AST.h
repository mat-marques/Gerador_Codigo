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
        list<ASTObject> globals;
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

class Variable {
    public:
        string category; //Global or Local
        string name;
        string type;
        vector<int> dimension; //Caso for vetor, matrix, etc - definir as dimensões do mesmo
};

class Constant {
    public:
        string name;
        string value;
};

class CallFunction {
    public:
        string funcName;
        list<Expression> params;
        void mipsCallFunction(ofstream & mipsFile, int label, string funcName);
};

class ASTObject {
    public:
        // IF, FOR, WHILE, DOWHILE, SCANF, PRINTF, RETURN, EXIT, ASSIGNMENT, CALLFUNCTION, VARIABLE, CONSTANTE
        void *statementClass;
        string className;
};

class Assignment {
    public:
        Variable variable;
        //Pode ser uma Expression ou um CallFunction
        ASTObject assignObject;
        void mipsAssignment(ofstream & mipsFile, int label, string funcName);
};

class Statement {
    public:
        list<ASTObject> statement;
        void mipsStatement(ofstream & mipsFile, int label, string funcName);
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
        string message;
        vector<char> variables_type;
        //Expression and CallFunction
        list<ASTObject> paramsList;
        void mipsPrintf(ofstream & mipsFile, int label, string funcName);
};

class Scanf {
    public:
        string message;
        vector<char> variables_type; 
        list<Expression> variables;
        void mipsScanf(ofstream & mipsFile, int label, string funcName);
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


class Expression {
    public:
        ASTObject *term;
        Expression *left; 
        string op; 
        Expression *right;
        vector<string> mipsExpression(ofstream & mipsFile);
        Variable *mipsMinimalMunch(ofstream & mipsFile, Expression *ex);
};

#endif
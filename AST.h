#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include "HashTable.h"

using namespace std;

class ASTObject;
class Expression;
class CondicionalExpression;
class BoolOperatorCondicional;
class DoWhile;
class While;
class IF;
class For;
class Scanf;
class Printf;
class Exit;
class Return;
class Statement;
class FunctionParam;
class Number;
class Constant;
class CallFunction;
class Function;
class Variable;
class Assignment;
class Program;
class StringsLabel;
class DataLabel;
class MipsInstruction;

extern list<MipsInstruction*> instructionList;
extern list<StringsLabel*> stringsLabelList;
extern list<DataLabel*> dataLabelList;

class Program {
    public:
        HashTable *symbolTable;
        list<ASTObject*> globals;
        list<Function*> functionsList;
        Program(){
            symbolTable = new HashTable(90);
        };
};

class FunctionParam{
    public:
        string name;
        string parm_type;
};

class Function{
    public:
        HashTable *symbolTable;
        string name;
        list<FunctionParam*> paramsList;
        string return_type;
        list<Statement*> statementList;
        Function(){
            symbolTable = new HashTable(90);
        };
        void mipsFunction(ofstream & mipsFile);
};

class Variable {
    public:
        string category; //Global or Local
        string name;
        string var_type;
        vector<int> dimension_size; //Caso for vetor, matrix, etc - definir as dimensões do mesmo
        vector<Expression*> dimension_exp;
        Variable(){};
};

class Number {
    public:
        string value; //Global or Local
        string number_type;
        Number(){};
};

class Constant {
    public:
        string name;
        string value;
        string const_type;
        Constant(){};
};

class CallFunction {
    public:
        string funcName;
        list<Expression*> params;
        string callfunc_type;
        vector<int> dimension_size;
        CallFunction(){};
        void mipsCallFunction(ofstream & mipsFile, string funcName);
};

class ASTObject {
    public:
        // IF, FOR, WHILE, DOWHILE, SCANF, PRINTF, RETURN, EXIT, ASSIGNMENT, CALLFUNCTION, VARIABLE, CONSTANTE
        void *statementClass;
        string className;
        ASTObject(){};
};

class Assignment {
    public:
        Variable variable;
        //Pode ser uma Expression ou um CallFunction
        ASTObject *assignObject;
        void mipsAssignment(ofstream & mipsFile, int label, string funcName);
        Assignment(){};
};

class Statement {
    public:
        list<ASTObject*> statement;
        void mipsStatement(ofstream & mipsFile, int label, string funcName);
        Statement(){};
};

class DoWhile {
    public:
        list<Statement*> statementList;
        BoolOperatorCondicional *condExp;
        void mipsDoWhile(ofstream & mipsFile, int label, string funcName);
        DoWhile(){};
};

class While {
    public:
        BoolOperatorCondicional *condExp;
        list<Statement*> statementList;
        void mipsWhile(ofstream & mipsFile, int label, string funcName);
        While(){};
};

class IF {
    public:
        BoolOperatorCondicional *condExp;
        list<Statement*> statementListThen;
        list<Statement*> statementListElse;
        void mipsIF(ofstream & mipsFile, int labelElse, string funcName);
        IF(){};
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
        BoolOperatorCondicional(){};
};

class CondicionalExpression {
    public:
        Expression *left; 
        string op;  // <, >, <=, >=. !=, ==
        Expression *right;
        CondicionalExpression(){};
};


class For {
    public:
        list<Expression*> valuesInitialization;
        BoolOperatorCondicional *condExp;
        list<Expression*> valuesAdjustment;
        list<Statement*> statementList;
        void mipsFor(ofstream & mipsFile, int label, string funcName);
        For(){};
};

class Printf {
    public:
        string message;
        vector<char> variables_type;
        //Expression and CallFunction
        list<ASTObject*> paramsList;
        void mipsPrintf(ofstream & mipsFile, int label, string funcName);
        Printf(){};
};

class Scanf {
    public:
        string message;
        vector<char> variables_type; 
        list<Expression*> variables;
        void mipsScanf(ofstream & mipsFile, int label, string funcName);
        Scanf(){};
};

class Exit {
    public:
        Expression *exp;
        void mipsExit(ofstream & mipsFile);
        Exit(){};
};

class Return {
    public:
        Expression *exp;
        void mipsReturn(ofstream & mipsFile);
        Return(){};
};

class Register {
    public:
        string name;
        string type;
        string tree;
        Register(){};
};

class Expression {
    public:
        ASTObject *term;
        Expression *left; 
        string op; 
        Expression *right;
        Register mipsExpression(ofstream & mipsFile);
        Register mipsMinimalMunch(ofstream & mipsFile, Expression *ex);
        Expression(){};
};

class StringsLabel{
    public:
        string label;
        string message;
        void mipsStringsLabel(ofstream & mipsFile) {
            mipsFile << this->label << ": ";
            mipsFile << ".asciiz " << "\"" << this->message << "\"\n";
        };
};

class DataLabel{
    public:
        string label;
        string data_type;
        string value;
        void mipsDataLabel(ofstream & mipsFile) {
            mipsFile << this->label << ": ";
            mipsFile << "." << this->data_type << ": " << this->value << "\n";
        };
};

class MipsInstruction{
    public:
        string label;
        string instruction;
        string register1;
        string register2;
        string register3;
        MipsInstruction(string label, string instruction, string register1, string register2, string register3){
            this->label = label;
            this->instruction= instruction;
            this->register1= register1;
            this->register2= register2;
            this->register3= register3;
        };
};

#endif
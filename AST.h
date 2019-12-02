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
class Register;
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
class StringsLabel;
class DataLabel;
class MipsInstruction;
class Program;

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
        void mipsProgram(ofstream & mipsFile);
        void mips();
        void allocateRegister(string funcName, string reg);
        Register allocateReg(string label, string funcName, std::list<MipsInstruction*>::iterator it);
        void storeReg(string reg1Name, string reg2Name,string label, string funcName, int index);
        void loadReg(string reg1Name, string reg2Name,string label, string funcName, int index);
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
        Statement* statementList;
        Function(){
            symbolTable = new HashTable(90);
            this->statementList = NULL;
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
        Register mipsVariableVector(ofstream & mipsFile, string funcName);
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
        Register mipsCallFunction(ofstream & mipsFile, string funcName);
};

class ASTObject {
    public:
        // IF, FOR, WHILE, DOWHILE, SCANF, PRINTF, RETURN, EXIT, ASSIGNMENT, CALLFUNCTION, VARIABLE, CONSTANTE
        void *statementClass;
        string className;
        ASTObject(){
        	this->statementClass = NULL;
        };
};

class Assignment {
    public:
        Variable *variable;
        //Pode ser uma Expression ou um CallFunction
        ASTObject *assignObject;
        void mipsAssignment(ofstream & mipsFile, int label, string funcName);
        Assignment(){
        	this->variable = NULL;
        	this->assignObject = NULL;
        };
};

class Statement {
    public:
        list<ASTObject*> statement;
        void mipsStatement(ofstream & mipsFile, int label, string funcName);
        Statement(){};
};

class DoWhile {
    public:
        Statement* statementList;
        BoolOperatorCondicional *condExp;
        void mipsDoWhile(ofstream & mipsFile, int label, string funcName);
        DoWhile(){
        	this->statementList = NULL;
        	this->condExp = NULL;
        };
};

class While {
    public:
        BoolOperatorCondicional *condExp;
        Statement* statementList;
        void mipsWhile(ofstream & mipsFile, int label, string funcName);
        While(){
        	this->statementList = NULL;
        	this->condExp = NULL;
        };
};

class IF {
    public:
        BoolOperatorCondicional *condExp;
        Statement* statementListThen;
        Statement* statementListElse;
        void mipsIF(ofstream & mipsFile, int labelElse, string funcName);
        IF(){
        	this->condExp = NULL;
        	this->statementListThen = NULL;
        	this->statementListElse = NULL;
        };
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
        BoolOperatorCondicional(){
        	this->boolOperator = "";
        	this->condExpLeft = NULL;
        	this->condExpRight = NULL;
        	this->nextBOC = NULL;
        };
};

class CondicionalExpression {
    public:
        Expression *left; 
        string op;  // <, >, <=, >=. !=, ==
        Expression *right;
        CondicionalExpression(){
        	this->op = "";
        	this->left = NULL;
        	this->right = NULL;
        };
};

class For {
    public:
        list<Assignment*> valuesInitialization;
        BoolOperatorCondicional *condExp;
        list<Assignment*> valuesAdjustment;
        Statement* statementList;
        void mipsFor(ofstream & mipsFile, int label, string funcName);
        For(){
        	this->statementList = NULL;
        	this->condExp = NULL;
        };
};

class Printf {
    public:
        vector<string> message;
        //Expression and CallFunction
        list<ASTObject*> paramsList;
        void mipsPrintf(ofstream & mipsFile, int label, string funcName);
        Printf(){
        };
};

class Scanf {
    public:
    	vector<string> message;
        list<Expression*> paramsList;
        void mipsScanf(ofstream & mipsFile, int label, string funcName);
        Scanf(){};
};

class Exit {
    public:
        Expression *exp;
        void mipsExit(ofstream & mipsFile);
        Exit(){
        	this->exp = exp;
        };
};

class Return {
    public:
        Expression *exp;
        void mipsReturn(ofstream & mipsFile);
        Return(){
        	this->exp = NULL;
        };
};

class Register {
    public:
        string name = "";
        string type = "";
        string vetor = "";
        string tree = "";
        string value = "";
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
        Expression(){
        	this->term = NULL; this->left = NULL; this->right = NULL; this->op = "";
        };
};

class StringsLabel{
    public:
        string label;
        string message;
        void mipsStringsLabel(ofstream & mipsFile) {
            mipsFile << this->label << ": ";
            mipsFile << ".asciiz " << "\"" << this->message << "\"\n";
        };
        void mips() {
            cout << this->label << ": ";
            cout << ".asciiz " << "\"" << this->message << "\"\n";
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
        string function;
        string instruction;
        string register1;
        string register2;
        string register3;
        MipsInstruction(string label, string function, string instruction, string register1, string register2, string register3){
            this->label = label;
            this->function = function;
            this->instruction= instruction;
            this->register1= register1;
            this->register2= register2;
            this->register3= register3;
        };
        void mips() {
        	cout << "  " << this->instruction;
        	if(this->register1 != "")
        		cout << " " << this->register1;
        	if(this->register2 != "")
        		cout << ", " << this->register2;
        	if(this->register3 != "")
        		cout << ", " << this->register3;
        	cout << "\n";
        };

};

#endif

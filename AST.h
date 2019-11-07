#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include "../lexico/HashTable.h"

using namespace std;

class Variable;
class Number;
class Program;
class Assign;
class Declaration;
class SimpleExpression;
class Expression;
class Block;
class DeclarationFunction;
class Term;
class Factor;
class Type;
class Statement;
class StatementWithoutLabel;
class FormalParms;
class Read;
class Write;

class Variable{
    public:
        string variable;
        Variable (string variable) { this->variable = variable; };
        void printVariable(ofstream & hFile);
};

class Number{
    public:
        string value;
        Number (string value) { this->value = value; };
        void printNumber(ofstream & hFile);
};

class Program {
    public:
        Variable *programName; vector<Variable*> *listVariables; Block *block;
        Program(Variable *programName, vector<Variable*> *listVariables, Block *block){
            this->programName = programName;
            this->listVariables = listVariables;
            this->block = block;
        };
        void printProgram(string fileName);
        void removeProgram();
        HashTable *createSymbolTable();
};

class Assign {
    public:
        Declaration *dec; Expression *ex;
        Assign(Declaration *dec, Expression *ex) {
            this->dec = dec;
            this->ex = ex;
        };
        void printAssign(ofstream & hFile);
        void removeAssign();
};

class BlockType {
    public:
        Variable *v; Type *t;
        BlockType(Variable *v, Type *t) {
            this->v = v;
            this->t = t;
        };
        void printBlockType(ofstream & hFile);
        void removeBlockType();
};

class BlockVar {
    public:
        vector<Variable*> *v; Type *t;
        BlockVar(vector<Variable*> *v, Type *t) {
            this->v = v;
            this->t = t;
        };
        void printBlockVar(ofstream & hFile);
        void removeBlockVar();
        int symbolTableBlockVar(HashTable &hash, int &scope, int displacement);
};

class Block {
    public:
        vector<Number*> *labels;
        vector<BlockType*> *varTypes;
        vector<BlockVar*> *varVariables;
        vector<DeclarationFunction*> *rotinas;
        vector<Statement*> *statements;
        
        Block (vector<Number*> *labels, vector<BlockType*> *varTypes, vector<BlockVar*> *varVariables, vector<DeclarationFunction*> *rotinas, vector<Statement*> *statements) { 
            this->labels = labels;
            this->varTypes = varTypes;
            this->varVariables = varVariables;
            this->rotinas = rotinas;
            this->statements = statements;
        };
        void printBlock(ofstream & hFile);
        void removeBlock();
        void symbolTableBlock(HashTable &hash, int &scope);
};

class Branch {
    public:
        Expression *ex; StatementWithoutLabel *st1; StatementWithoutLabel *st2;
        Branch(Expression *ex, StatementWithoutLabel *st1, StatementWithoutLabel *st2){
            this->ex = ex;
            this->st1 = st1;
            this->st2 = st2;
        };
        void printBranch(ofstream & hFile);
        void removeBranch();
};

class Declaration {
    public:
        Variable *v; vector<Expression*> *ex;
        Declaration(Variable *v, vector<Expression*> *ex) {
            this->v = v;
            this->ex = ex;
        };
        void printDeclaration(ofstream & hFile);
        void removeDeclaration();
};

class DeclarationFunction{
    public:
        Variable *functionName; vector<FormalParms*> *listParams; Variable *functionType; Block *block;
        DeclarationFunction(Variable *functionName, vector<FormalParms*>  *listParams, Variable *functionType, Block *block) {
            this->functionName = functionName;
            this->listParams = listParams;
            this->functionType = functionType;
            this->block = block;
        };
        void printDeclarationFunction(ofstream & hFile);
        void removeDeclarationFunction();
        void symbolTableDeclarationFunction(HashTable &hash, int &scope, int displacement);
};

class SimpleExpression {
    public:
        string op; Term* t; SimpleExpression *simpleExpressions;
        SimpleExpression(string op, Term *t, SimpleExpression *simpleExpressions) {
            this->op = op;
            this->t = t;
            this->simpleExpressions = simpleExpressions;
        };
        void printSimpleExpression(ofstream & hFile);
        void removeSimpleExpression();
};

class Expression {
    public:
        SimpleExpression *ex1; string op; SimpleExpression *ex2;
        Expression(SimpleExpression *ex1, string op, SimpleExpression *ex2) {
            this->ex1 = ex1;
            this->op = op;
            this->ex2 = ex2;
        };
        void printExpression(ofstream & hFile);
        void removeExpression();
};

class Factor {
    public:
        Declaration *d;
        Number *n;
        Expression *ex;
        Factor *f;
        Factor(Declaration *d, Number *n, Expression *ex, Factor *f) {
            this->d = d;
            this->n = n;
            this->ex = ex;
            this->f = f;
        };
        void printFactor(ofstream & hFile);
        void removeFactor();
};

class Term {
    public:
        Factor *f; string op; Term *t;
        Term(Factor *f, string op, Term *t){ 
            this->f = f;
            this->op = op;
            this->t = t;
        };
        void printTerm(ofstream & hFile);
        void removeTerm();
};

class FormalParms {
    public:
        string op; vector<Variable*> *variables; string type;
        FormalParms(string op, vector<Variable*> *variables, string type){
            this->op = op;
            this->variables = variables;
            this->type = type;
        };
        void printFormalParms(ofstream & hFile);
        void removeFormalParms();
        int symbolTableFormalParms(HashTable &hash, int &scope, int displacement);
};

class Loop {
    public:
        Expression *ex; StatementWithoutLabel *statement;
        Loop(Expression *ex, StatementWithoutLabel *statement){
            this->ex = ex;
            this->statement = statement;
        }
        void printLoop(ofstream & hFile);
        void removeLoop();
};

class Type {
    public:
        Variable *type; vector<Number*> *arrayNumbers; Type *types;
        Type(Variable *type, vector<Number*> *arrayNumbers, Type *types) {
            this->type = type;
            this->arrayNumbers = arrayNumbers;
            this->types = types;
        };
        void printType(ofstream & hFile);
        void removeType();
};

class StatementWithoutLabel {
    public:
        Assign *assign;
        Declaration *declaration;
        Number *goto_;
        vector<Statement*> *statements;
        Branch *branch;
        Loop *loop;
        Read *read;
        Write *write;
        StatementWithoutLabel (Assign *assign, Declaration *declaration, 
        Number *goto_, vector<Statement*> *statements, Branch *branch, Loop *loop,
		Read *read, Write *write) {
            this->assign = assign;
            this->declaration = declaration;
            this->goto_ = goto_;
            this->statements = statements;
            this->branch = branch;
            this->loop = loop;
            this->read = read;
            this->write = write;
        };
        void printStatementWithoutLabel(ofstream & hFile);
        void removeStatementWithoutLabel();
};

class Statement {
    public:
        Number *label; StatementWithoutLabel *st;
        Statement(Number *label, StatementWithoutLabel *st) {
            this->label = label;
            this->st = st;
        };
        void printStatement(ofstream & hFile);
        void removeStatement();
};

class Read {
	public:
		vector<Variable*> *variables;
		Read(vector<Variable*> *variables){
			this->variables = variables;
		};
		void printRead(ofstream & hFile);
		void removeRead();
};

class Write {
	public:
		vector<Expression*> *expressions;
		Write(vector<Expression*> *expressions){
			this->expressions = expressions;
		};
		void printWrite(ofstream & hFile);
		void removeWrite();
};


#endif

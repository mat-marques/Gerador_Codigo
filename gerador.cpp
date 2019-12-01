#include <iostream>
#include <string>
#include "Arquivo.h"
#include "AST.h"

using namespace std;

int main() {
	ofstream mipsFile;
    mipsFile.open ("MipsSaida.txt");
//   string input;
//   vector<string> file_input;
//   //Leitura da entrada
//   while(getline(cin, input)) {
//      file_input.push_back(input);
//   }
//   Arquivo *file = new Arquivo(file_input);

   Program *p = new Program();
   Function *main_ = new Function();
   p->functionsList.push_back(main_);
   main_->name = "main";
   main_->return_type = "INT";
   
   Variable *var = new Variable();
   var->name = "n";
   var->var_type = "INT";
   ASTObject *as = new ASTObject();
   as->className = "VARIABLE";
   as->statementClass = var;

   Number *n1 = new Number();
   Number *n2 = new Number();
   Number *n3 = new Number();

   n1->value = "10";
   n1->number_type = "INT";
   n2->value = "10";
   n2->number_type = "INT";
   n3->value = "10";
   n3->number_type = "INT";

   Assignment *assig = new Assignment();
   assig->variable = var;

   ASTObject *ast_ex = new ASTObject();
   ASTObject *ast_term1 = new ASTObject();
   ASTObject *ast_term2 = new ASTObject();
   ASTObject *ast_term3 = new ASTObject();
   ASTObject *ast_term4 = new ASTObject();

   Expression *ex = new Expression();
   Expression *ex2 = new Expression();
   Expression *ex3 = new Expression();
   Expression *ex4 = new Expression();
   Expression *ex5 = new Expression();
   Expression *ex6 = new Expression();
   Expression *ex7 = new Expression();

   Variable *v = new Variable();
   v->name = "a";
   v->var_type = "INT";
   v->dimension_size.push_back(10);
   Expression *ex_vector = new Expression();
   Expression *ex_vector_l = new Expression();
   Expression *ex_vector_r = new Expression();
   ASTObject *vector_term1 = new ASTObject();
   ASTObject *vector_term2 = new ASTObject();

   Variable *v_vector = new Variable();
   v_vector->name = "i"; v_vector->var_type = "INT";
   Number *n1_vector = new Number();
   n1_vector->value = "1"; n1_vector->number_type = "INT";
   v->dimension_exp.push_back(ex_vector);
   ex_vector->op = "+";
   ex_vector->left = ex_vector_l;
   ex_vector->right = ex_vector_r;
   ex_vector_l->term = vector_term1;
   ex_vector_r->term = vector_term2;
   vector_term1->statementClass = static_cast<void*>(v_vector);
   vector_term1->className = "VARIABLE";
   vector_term2->statementClass = static_cast<void*>(n1_vector);
   vector_term2->className = "NUMBER";

   ex->op = "+";
   ex->left = ex2;
   ex->right = ex3;
   ex2->term = ast_term1;
   ex3->op = "+";
   ex3->left = ex4;
   ex3->right = ex5;
   ex4->term = ast_term2;
   ex5->op = "+";
   ex5->left = ex6;
   ex5->right = ex7;
   ex6->term = ast_term3;
   ex7->term = ast_term4;


   ast_ex->statementClass = static_cast<void*>( ex );
   ast_ex->className = "EXPRESSION";

   ast_term1->statementClass = static_cast<void*>( n1 );
   ast_term1->className = "NUMBER";
   ast_term2->statementClass = static_cast<void*>( n2 );
   ast_term2->className = "NUMBER";
   ast_term3->statementClass = static_cast<void*>( v );
   ast_term3->className = "VARIABLE";
   ast_term4->statementClass = static_cast<void*>( n3 );
   ast_term4->className = "NUMBER";

   assig->assignObject = ast_ex;

   Assignment *assig2 = new Assignment();
   assig2->variable = v;
   ASTObject *ast_2assig2 = new ASTObject();
   ast_2assig2->className = "EXPRESSION";
   ast_2assig2->statementClass = static_cast<void*>(ex6);
   assig2->assignObject = ast_2assig2;

   ASTObject *ast_assig = new ASTObject();
   ASTObject *ast_assig2 = new ASTObject();
   ast_assig->statementClass = static_cast<void*>(assig);
   ast_assig2->statementClass = static_cast<void*>(assig2);
   ast_assig->className = "ASSIGNMENT";
   ast_assig2->className = "ASSIGNMENT";

   Exit *exit_ = new Exit();
   Return *return_ = new Return();

   exit_->exp = ex;
   return_->exp = ex;
   ASTObject *ast_assig3 = new ASTObject();
   ASTObject *ast_assig4 = new ASTObject();
   ast_assig3->statementClass = static_cast<void*>(exit_);
   ast_assig3->className = "EXIT";
   ast_assig4->statementClass = static_cast<void*>(return_);
   ast_assig4->className = "RETURN";

   CallFunction *cf = new CallFunction();
   cf->callfunc_type = "INT";
   cf->funcName = "Teste";
   cf->params.push_back(ex);
   cf->params.push_back(ex);
   cf->params.push_back(ex);
   cf->params.push_back(ex);
   ASTObject *ast_assig5 = new ASTObject();
   ast_assig5->statementClass = static_cast<void*>(cf);
   ast_assig5->className = "CALLFUNCTION";


   Printf *pf = new Printf();

   pf->message.push_back("Valores ");
   pf->message.push_back("%d");
   pf->message.push_back("\n");
   pf->paramsList.push_back(ast_ex);
   ASTObject *ast_assig6 = new ASTObject();
   ast_assig6->statementClass = static_cast<void*>(pf);
   ast_assig6->className = "PRINTF";

   Scanf *sf = new Scanf();
   sf->message.push_back("%d");
   Expression *ex_2 = new Expression();
   ASTObject *ast_assig_01 = new ASTObject();
   ast_assig_01->statementClass = static_cast<void*>(var);
   ast_assig_01->className = "VARIABLE";
   ex_2->term = ast_assig_01;
   sf->paramsList.push_back(ex_2);
   ASTObject *ast_assig7 = new ASTObject();
   ast_assig7->statementClass = static_cast<void*>(sf);
   ast_assig7->className = "SCANF";

   IF *if_ = new IF();
   BoolOperatorCondicional *bop = new BoolOperatorCondicional();
   if_->condExp = bop;
   CondicionalExpression *co = new CondicionalExpression();
   co->left = ex_2;
   co->op = "<";
   co->right = ex_2;

   bop->condExpLeft = co;
   bop->boolOperator = "&&";
   bop->condExpRight = co;

   Statement *s3 = new Statement();
   if_->statementListThen = s3;
   if_->statementListElse = s3;
   s3->statement.push_back(ast_assig6);
   s3->statement.push_back(ast_assig7);

   ASTObject *ast_assig8 = new ASTObject();
   ast_assig8->statementClass = static_cast<void*>(if_);
   ast_assig8->className = "IF";

   DoWhile *dow = new DoWhile();

   dow->condExp = bop;
   dow->statementList = s3;

   ASTObject *ast_assig9 = new ASTObject();
   ast_assig9->statementClass = static_cast<void*>(dow);
   ast_assig9->className = "DOWHILE";

   While *whi = new While();
   whi->condExp = bop;
   whi->statementList = s3;

   ASTObject *ast_assig10 = new ASTObject();
   ast_assig10->statementClass = static_cast<void*>(whi);
   ast_assig10->className = "WHILE";

   For *fo = new For();
   fo->valuesInitialization.push_back(assig2);
   fo->condExp = bop;
   fo->statementList = s3;
   fo->valuesAdjustment.push_back(assig2);

   ASTObject *ast_assig11 = new ASTObject();
   ast_assig11->statementClass = static_cast<void*>(fo);
   ast_assig11->className = "FOR";

   Statement *s2 = new Statement();
   s2->statement.push_back(as);
// s2->statement.push_back(ast_assig);
   s2->statement.push_back(ast_assig2);
   s2->statement.push_back(ast_assig3);
   s2->statement.push_back(ast_assig4);
   s2->statement.push_back(ast_assig5);
   s2->statement.push_back(ast_assig6);
   s2->statement.push_back(ast_assig7);
   s2->statement.push_back(ast_assig8);
   s2->statement.push_back(ast_assig9);
   s2->statement.push_back(ast_assig10);
   s2->statement.push_back(ast_assig11);
   main_->statementList = s2;

   p->mipsProgram(mipsFile);


   mipsFile.close();
   cout << "FIM" <<endl;
   return 0;

}


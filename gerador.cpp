#include <iostream>
#include <string>
#include "Arquivo.h"
#include "AST.h"

using namespace std;

int main() {
	ofstream mipsFile;
    mipsFile.open ("MipsIntermediario.txt");
//   string input;
//   vector<string> file_input;
//   //Leitura da entrada
//   while(getline(cin, input)) {
//      file_input.push_back(input);
//   }
//   Arquivo *file = new Arquivo(file_input);

   Program *p = new Program();


   Function *fatorial = new Function();
   fatorial->name = "fatorial";
   fatorial->return_type = "INT";
   FunctionParam *n = new FunctionParam();
   n->name = "n";
   n->parm_type = "INT";
   fatorial->paramsList.push_back(n);

   IF *if1 = new IF();
   BoolOperatorCondicional *boop_if1 = new BoolOperatorCondicional();
   CondicionalExpression *con_if1 = new CondicionalExpression();

   Expression *ex_if1_l = new Expression();
   Variable *var_if1 = new Variable();
   var_if1->name = "n";
   var_if1->var_type = "INT";
   ASTObject *ast_if1_l = new ASTObject();
   ast_if1_l->statementClass = static_cast<void*>(var_if1);
   ast_if1_l->className = "VARIABLE";
   ex_if1_l->term = ast_if1_l;
   con_if1->left = ex_if1_l;

   con_if1->op = "<=";

   Expression *ex_if1_r = new Expression();
   Number *num_if1 = new Number();
   num_if1->value = "0";
   num_if1->number_type = "INT";
   ASTObject *ast_if1_r = new ASTObject();
   ast_if1_r->statementClass = static_cast<void*>(num_if1);
   ast_if1_r->className = "NUMBER";
   ex_if1_r->term = ast_if1_r;
   con_if1->right = ex_if1_r;

   boop_if1->condExpLeft = con_if1;

   if1->condExp = boop_if1;

   Statement *s_if1 = new Statement();
   Return *ret_if1 = new Return();
   ret_if1->exp = ex_if1_r; // Number == 0
   ASTObject *ast_ret_if1 = new ASTObject();
   ast_ret_if1->statementClass = static_cast<void*>(ret_if1);
   ast_ret_if1->className = "RETURN";
   s_if1->statement.push_back(ast_ret_if1);
   if1->statementListThen = s_if1;


   IF *if2 = new IF();
   BoolOperatorCondicional *boop_if2 = new BoolOperatorCondicional();
   CondicionalExpression *con_if2 = new CondicionalExpression();

   con_if2->left = ex_if1_l;
   con_if2->op = "==";

   Expression *ex_if2_r = new Expression();
   Number *num_if2 = new Number();
   num_if2->value = "1";
   num_if2->number_type = "INT";
   ASTObject *ast_if2_r = new ASTObject();
   ast_if2_r->statementClass = static_cast<void*>(num_if2);
   ast_if2_r->className = "NUMBER";
   ex_if2_r->term = ast_if2_r;
   con_if2->right = ex_if2_r;

   boop_if2->condExpLeft = con_if2;

   if2->condExp = boop_if2;

   Statement *s_if2_then = new Statement();
   Return *ret_if2 = new Return();
   ret_if2->exp = ex_if2_r; // Number == 0
   ASTObject *ast_ret_if2_then = new ASTObject();
   ast_ret_if2_then->statementClass = static_cast<void*>(ret_if2);
   ast_ret_if2_then->className = "RETURN";
   s_if2_then->statement.push_back(ast_ret_if2_then);
   if2->statementListThen = s_if2_then;

   Statement *s_if2_else = new Statement();
   Return *ret_if3 = new Return();
   //n * fatorial(n-1);
   Expression *ex_if2_else1 = new Expression();
   Expression *ex_if2_else2 = new Expression();
   Expression *ex_if2_else3 = new Expression();

   ex_if2_else1->op = "*";
   ex_if2_else1->left = ex_if2_else2;
   ex_if2_else1->right = ex_if2_else3;
   ex_if2_else2->term = ast_if1_l;
   CallFunction *fat = new CallFunction();
   fat->callfunc_type = "INT";
   fat->funcName = "fatorial";
   Expression *ex_fat_1 = new Expression();
   Expression *ex_fat_2 = new Expression();
   Expression *ex_fat_3 = new Expression();
   ex_fat_1->op = "-";
   ex_fat_1->left = ex_fat_2;
   ex_fat_1->right = ex_fat_3;
   ex_fat_2->term = ast_if1_l;
   ex_fat_3->term = ast_if2_r;
   fat->params.push_back(ex_fat_1);
   ASTObject *ast_fat = new ASTObject();
   ast_fat->statementClass = static_cast<void*>(fat);
   ast_fat->className = "CALLFUNCTION";
   ex_if2_else3->term = ast_fat;

   ret_if3->exp = ex_if2_else1;
   ASTObject *ast_ret_if2_else = new ASTObject();
   ast_ret_if2_else->statementClass = static_cast<void*>(ret_if3);
   ast_ret_if2_else->className = "RETURN";
   s_if2_else->statement.push_back(ast_ret_if2_else);
   if2->statementListElse = s_if2_else;

   ASTObject *ast_if1 = new ASTObject();
   ast_if1->statementClass = static_cast<void*>(if1);
   ast_if1->className = "IF";

   ASTObject *ast_if2 = new ASTObject();
   ast_if2->statementClass = static_cast<void*>(if2);
   ast_if2->className = "IF";

   Statement *s_fatorial = new Statement();
   s_fatorial->statement.push_back(ast_if1);
   s_fatorial->statement.push_back(ast_if2);
   fatorial->statementList = s_fatorial;

   Function *main_ = new Function();
   main_->name = "main";
   main_->return_type = "INT";
   Statement *s_main = new Statement();

   Printf *pf = new Printf();
   pf->message.push_back("Entre com um valor inteiro:");
   ASTObject *ast_pf = new ASTObject();
   ast_pf->statementClass = static_cast<void*>(pf);
   ast_pf->className = "PRINTF";

   Scanf *sf = new Scanf();
   sf->message.push_back("%d");
   sf->paramsList.push_back(ex_if1_l);
   ASTObject *ast_sf = new ASTObject();
   ast_sf->statementClass = static_cast<void*>(sf);
   ast_sf->className = "SCANF";

   Printf *pf2 = new Printf();
   pf2->message.push_back("O fatorial de ");
   pf2->message.push_back("%d");
   pf2->message.push_back("eh: ");
   pf2->message.push_back("%d");
   pf2->message.push_back("\n");

   ASTObject *ast_pf2_1 = new ASTObject();
   ast_pf2_1->statementClass = static_cast<void*>(ex_if1_l);
   ast_pf2_1->className = "EXPRESSION";
   pf2->paramsList.push_back(ast_pf2_1);

   CallFunction *fat2 = new CallFunction();
   fat2->callfunc_type = "INT";
   fat2->funcName = "fatorial";
   fat2->params.push_back(ex_if1_l);
   ASTObject *ast_pf2_2 = new ASTObject();
   ast_pf2_2->statementClass = static_cast<void*>(fat2);
   ast_pf2_2->className = "CALLFUNCTION";
   pf2->paramsList.push_back(ast_pf2_2);

   ASTObject *ast_pf2 = new ASTObject();
   ast_pf2->statementClass = static_cast<void*>(pf2);
   ast_pf2->className = "PRINTF";

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
  assig->variable = var_if1;

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

  ex->op = "~";
  ex->left = ex2;
//  ex->right = ex3;
  ex2->term = ast_term3;
//  ex3->term = ast_term1;
//  ex3->op = "+";
//  ex3->left = ex4;
//  ex3->right = ex5;
//  ex4->term = ast_term2;
//  ex5->op = "+";
//  ex5->left = ex6;
//  ex5->right = ex7;
//  ex6->term = ast_term3;
//  ex7->term = ast_term4;


  ast_ex->statementClass = static_cast<void*>( ex );
  ast_ex->className = "EXPRESSION";

  ast_term1->statementClass = static_cast<void*>( n1 );
  ast_term1->className = "NUMBER";
  ast_term2->statementClass = static_cast<void*>( n2 );
  ast_term2->className = "NUMBER";
  ast_term3->statementClass = static_cast<void*>( var_if1 );
  ast_term3->className = "VARIABLE";
  ast_term4->statementClass = static_cast<void*>( n3 );
  ast_term4->className = "NUMBER";

  assig->assignObject = ast_ex;


  ASTObject *ast_assig = new ASTObject();
  ast_assig->statementClass = static_cast<void*>(assig);
  ast_assig->className = "ASSIGNMENT";


   s_main->statement.push_back(ast_if1_l);
   s_main->statement.push_back(ast_pf);
   s_main->statement.push_back(ast_sf);
   s_main->statement.push_back(ast_pf2);
   s_main->statement.push_back(ast_assig);
//   s_main->statement.push_back(ast_ret_if1);
   main_->statementList = s_main;

   
//
//   Variable *var = new Variable();
//   var->name = "n";
//   var->var_type = "INT";
//   ASTObject *as = new ASTObject();
//   as->className = "VARIABLE";
//   as->statementClass = var;
//
//   Number *n1 = new Number();
//   Number *n2 = new Number();
//   Number *n3 = new Number();
//
//   n1->value = "10";
//   n1->number_type = "INT";
//   n2->value = "10";
//   n2->number_type = "INT";
//   n3->value = "10";
//   n3->number_type = "INT";
//
//   Assignment *assig = new Assignment();
//   assig->variable = var;
//
//   ASTObject *ast_ex = new ASTObject();
//   ASTObject *ast_term1 = new ASTObject();
//   ASTObject *ast_term2 = new ASTObject();
//   ASTObject *ast_term3 = new ASTObject();
//   ASTObject *ast_term4 = new ASTObject();
//
//   Expression *ex = new Expression();
//   Expression *ex2 = new Expression();
//   Expression *ex3 = new Expression();
//   Expression *ex4 = new Expression();
//   Expression *ex5 = new Expression();
//   Expression *ex6 = new Expression();
//   Expression *ex7 = new Expression();
//
//   Variable *v = new Variable();
//   v->name = "a";
//   v->var_type = "INT";
//   v->dimension_size.push_back(10);
//   Expression *ex_vector = new Expression();
//   Expression *ex_vector_l = new Expression();
//   Expression *ex_vector_r = new Expression();
//   ASTObject *vector_term1 = new ASTObject();
//   ASTObject *vector_term2 = new ASTObject();
//
//   Variable *v_vector = new Variable();
//   v_vector->name = "i"; v_vector->var_type = "INT";
//   Number *n1_vector = new Number();
//   n1_vector->value = "1"; n1_vector->number_type = "INT";
//   v->dimension_exp.push_back(ex_vector);
//   ex_vector->op = "+";
//   ex_vector->left = ex_vector_l;
//   ex_vector->right = ex_vector_r;
//   ex_vector_l->term = vector_term1;
//   ex_vector_r->term = vector_term2;
//   vector_term1->statementClass = static_cast<void*>(v_vector);
//   vector_term1->className = "VARIABLE";
//   vector_term2->statementClass = static_cast<void*>(n1_vector);
//   vector_term2->className = "NUMBER";
//
//   ex->op = "+";
//   ex->left = ex2;
//   ex->right = ex3;
//   ex2->term = ast_term1;
//   ex3->op = "+";
//   ex3->left = ex4;
//   ex3->right = ex5;
//   ex4->term = ast_term2;
//   ex5->op = "+";
//   ex5->left = ex6;
//   ex5->right = ex7;
//   ex6->term = ast_term3;
//   ex7->term = ast_term4;
//
//
//   ast_ex->statementClass = static_cast<void*>( ex );
//   ast_ex->className = "EXPRESSION";
//
//   ast_term1->statementClass = static_cast<void*>( n1 );
//   ast_term1->className = "NUMBER";
//   ast_term2->statementClass = static_cast<void*>( n2 );
//   ast_term2->className = "NUMBER";
//   ast_term3->statementClass = static_cast<void*>( v );
//   ast_term3->className = "VARIABLE";
//   ast_term4->statementClass = static_cast<void*>( n3 );
//   ast_term4->className = "NUMBER";
//
//   assig->assignObject = ast_ex;
//
//   Assignment *assig2 = new Assignment();
//   assig2->variable = v;
//   ASTObject *ast_2assig2 = new ASTObject();
//   ast_2assig2->className = "EXPRESSION";
//   ast_2assig2->statementClass = static_cast<void*>(ex6);
//   assig2->assignObject = ast_2assig2;
//
//   ASTObject *ast_assig = new ASTObject();
//   ASTObject *ast_assig2 = new ASTObject();
//   ast_assig->statementClass = static_cast<void*>(assig);
//   ast_assig2->statementClass = static_cast<void*>(assig2);
//   ast_assig->className = "ASSIGNMENT";
//   ast_assig2->className = "ASSIGNMENT";
//
//   Exit *exit_ = new Exit();
//   Return *return_ = new Return();
//
//   exit_->exp = ex;
//   return_->exp = ex;
//   ASTObject *ast_assig3 = new ASTObject();
//   ASTObject *ast_assig4 = new ASTObject();
//   ast_assig3->statementClass = static_cast<void*>(exit_);
//   ast_assig3->className = "EXIT";
//   ast_assig4->statementClass = static_cast<void*>(return_);
//   ast_assig4->className = "RETURN";
//
//   CallFunction *cf = new CallFunction();
//   cf->callfunc_type = "INT";
//   cf->funcName = "Teste";
//   cf->params.push_back(ex);
//   cf->params.push_back(ex);
//   cf->params.push_back(ex);
//   cf->params.push_back(ex);
//   ASTObject *ast_assig5 = new ASTObject();
//   ast_assig5->statementClass = static_cast<void*>(cf);
//   ast_assig5->className = "CALLFUNCTION";
//
//
//   Printf *pf = new Printf();
//
//   pf->message.push_back("Valores ");
//   pf->message.push_back("%d");
//   pf->message.push_back("\n");
//   pf->paramsList.push_back(ast_ex);
//   ASTObject *ast_assig6 = new ASTObject();
//   ast_assig6->statementClass = static_cast<void*>(pf);
//   ast_assig6->className = "PRINTF";
//
//   Scanf *sf = new Scanf();
//   sf->message.push_back("%d");
//   Expression *ex_2 = new Expression();
//   ASTObject *ast_assig_01 = new ASTObject();
//   ast_assig_01->statementClass = static_cast<void*>(var);
//   ast_assig_01->className = "VARIABLE";
//   ex_2->term = ast_assig_01;
//   sf->paramsList.push_back(ex_2);
//   ASTObject *ast_assig7 = new ASTObject();
//   ast_assig7->statementClass = static_cast<void*>(sf);
//   ast_assig7->className = "SCANF";
//
//   IF *if_ = new IF();
//   BoolOperatorCondicional *bop = new BoolOperatorCondicional();
//   if_->condExp = bop;
//   CondicionalExpression *co = new CondicionalExpression();
//   co->left = ex_2;
//   co->op = "<";
//   co->right = ex_2;
//
//   bop->condExpLeft = co;
//   bop->boolOperator = "&&";
//   bop->condExpRight = co;
//
//   Statement *s3 = new Statement();
//   if_->statementListThen = s3;
//   if_->statementListElse = s3;
//   s3->statement.push_back(ast_assig6);
//   s3->statement.push_back(ast_assig7);
//
//   ASTObject *ast_assig8 = new ASTObject();
//   ast_assig8->statementClass = static_cast<void*>(if_);
//   ast_assig8->className = "IF";
//
//   DoWhile *dow = new DoWhile();
//
//   dow->condExp = bop;
//   dow->statementList = s3;
//
//   ASTObject *ast_assig9 = new ASTObject();
//   ast_assig9->statementClass = static_cast<void*>(dow);
//   ast_assig9->className = "DOWHILE";
//
//   While *whi = new While();
//   whi->condExp = bop;
//   whi->statementList = s3;
//
//   ASTObject *ast_assig10 = new ASTObject();
//   ast_assig10->statementClass = static_cast<void*>(whi);
//   ast_assig10->className = "WHILE";
//
//   For *fo = new For();
//   fo->valuesInitialization.push_back(assig2);
//   fo->condExp = bop;
//   fo->statementList = s3;
//   fo->valuesAdjustment.push_back(assig2);
//
//   ASTObject *ast_assig11 = new ASTObject();
//   ast_assig11->statementClass = static_cast<void*>(fo);
//   ast_assig11->className = "FOR";
//
//   Statement *s2 = new Statement();
//   s2->statement.push_back(as);
//// s2->statement.push_back(ast_assig);
//   s2->statement.push_back(ast_assig2);
//   s2->statement.push_back(ast_assig3);
//   s2->statement.push_back(ast_assig4);
//   s2->statement.push_back(ast_assig5);
//   s2->statement.push_back(ast_assig6);
//   s2->statement.push_back(ast_assig7);
//   s2->statement.push_back(ast_assig8);
//   s2->statement.push_back(ast_assig9);
//   s2->statement.push_back(ast_assig10);
//   s2->statement.push_back(ast_assig11);
//   main_->statementList = s2;

   p->functionsList.push_back(fatorial);
   p->functionsList.push_back(main_);

   p->mipsProgram(mipsFile);

   mipsFile.close();
//   p->mips();
   p->allocateRegister("fatorial", "R0");
   p->mips();

   return 0;

}


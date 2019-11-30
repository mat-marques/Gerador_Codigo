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
   Statement *s2 = new Statement();
   s2->statement.push_back(as);
//   s2->statement.push_back(ast_assig);
   s2->statement.push_back(ast_assig2);
   main_->statementList = s2;

   p->mipsProgram(mipsFile);

   mipsFile.close();
   cout << "FIM" <<endl;
   return 0;

}


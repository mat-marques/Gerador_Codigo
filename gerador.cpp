#include <iostream>
#include <string>
#include "AST.h"

using namespace std;

int main() {
   Program *p = new Program();
   Function *fatorial = new Function();
   Function *main_ = new Function();

   p->functionsList.push_back(fatorial);
   p->functionsList.push_back(main_);

   fatorial->name = "fatorial";
   fatorial->return_type = "INT";

   FunctionParam *parm = new FunctionParam();
   parm->name = "n";
   parm->parm_type = "INT";

   fatorial->paramsList.push_back(parm);



   main_->name = "main";
   main_->return_type = "INT";
   
   Variable *var = new Variable();
   var->name = "n";
   var->var_type = "INT";
   ASTObject *as = new ASTObject();
   as->className = "VARIABLE";
   as->statementClass = var;

   Printf *pf1 = new Printf();
   pf1->message = "Entre com um valor inteiro:";
   ASTObject *aspf1 = new ASTObject();
   aspf1->statementClass = pf1;
   aspf1->className = "PRINTF";

   Scanf *sc = new Scanf();
   sc->message = "%d";
   sc->variables_type.push_back('d');
   Variable *var_nnn = new Variable();
   var_nnn->name = "n";
   var_nnn->var_type = "INT";
   Expression *exsc = new Expression();
   ASTObject *assf = new ASTObject();
   assf->statementClass = var_nnn;
   assf->className = "VARIABLE";
   exsc->term = assf;
   sc->variables.push_back(exsc);
   
   ASTObject *sf1 = new ASTObject();
   sf1->statementClass = sc;
   sf1->className = "SCANF";

   Printf *pf2 = new Printf();
   pf2->message = "O fatorial de %d eh: %d\n";
   pf2->variables_type.push_back('d');
   pf2->variables_type.push_back('d');
   ASTObject *aspf_2 = new ASTObject();
   aspf_2->statementClass = pf2;
   aspf_2->className = "PRINTF";

   ASTObject *aspf2 = new ASTObject();
   ASTObject *aspf3 = new ASTObject();

   Variable *var_n = new Variable();
   var_n->name = "n";
   var_n->var_type = "INT";
   aspf2->statementClass = var_n;
   aspf2->className = "VARIABLE";

   CallFunction *cf = new CallFunction();
   cf->funcName = "fatorial";
   Expression *expf = new Expression();
   Variable *var_nn = new Variable();
   var_nn->name = "n";
   var_nn->var_type = "INT";
   ASTObject *aspf4 = new ASTObject();
   aspf4->statementClass = var_nn;
   aspf4->className = "VARIABLE";
   expf->term = aspf4;
   cf->params.push_back(expf);
   aspf3->statementClass = cf;
   aspf3->className = "CALLFUNCTION";

   pf2->paramsList.push_back(aspf2);
   pf2->paramsList.push_back(aspf3);

   Return *r = new Return();
   Expression *exr = new Expression();
   ASTObject *asr = new ASTObject();
   Number *numb = new Number();
   numb->value = "0";
   numb->number_type = "INT";
   asr->statementClass = numb;
   asr->className = "NUMBER";
   exr->term = asr;
   r->exp = exr;
   ASTObject *asrr = new ASTObject();
   asrr->statementClass = r;
   asrr->className = "RETURN";


   Statement *s = new Statement();
   s->statement.push_back(as);
   s->statement.push_back(aspf1);
   s->statement.push_back(sf1);
   s->statement.push_back(aspf_2);
   s->statement.push_back(asrr);
   main_->statementList.push_back(s);

   return 0;
}


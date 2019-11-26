#include "AST.h"

class StringsLabel{
    public:
        string label;
        string message;
        void mipsStringsLabel(ofstream & mipsFile) {
            mipsFile << this->label << ": ";
            mipsFile << ".asciiz " << "\"" << this->message << "\"\n";
        };
};

list<StringsLabel*> stringsLabelList;

/*
*       MIPS PARA A Function
*/
void Function::mipsFunction(ofstream & mipsFile){
    mipsFile << this->name << ":\n";
    for (std::list<Statement>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        it->mipsStatement(mipsFile, 0, this->name);
    }

    for (std::list<StringsLabel*>::iterator it = stringsLabelList.begin(); it != stringsLabelList.end(); ++it) {
        (*it)->mipsStringsLabel(mipsFile);
    }
}

/*
*       MIPS PARA O Statement
*/
void Statement::mipsStatement(ofstream & mipsFile, int label, string funcName){
    int ifLabel = label,forLabel = label, whileLabel = label, dowhileLabel = label, assignmentLabel = label,
    callfunctionLabel = label, printfLabel = label, scanfLabel = label;
    
    for (std::list<ASTObject>::iterator it = this->statement.begin(); it != this->statement.end(); ++it) {
        if(it->className == "IF"){
            IF *ifObject = static_cast<IF*>( it->statementClass );
            ifObject->mipsIF(mipsFile, ifLabel, funcName);
            ifLabel++;
        }
        else if(it->className == "FOR"){
            For *forObject = static_cast<For*>( it->statementClass );
            forObject->mipsFor(mipsFile, forLabel, funcName);
            forLabel++;
        }
        else if(it->className == "WHILE"){
            While *whileObject = static_cast<While*>( it->statementClass );
            whileObject->mipsWhile(mipsFile, whileLabel, funcName);
            whileLabel++;
        }
        else if(it->className == "DOWHILE"){
            DoWhile *dowhileObject = static_cast<DoWhile*>( it->statementClass );
            dowhileObject->mipsDoWhile(mipsFile, dowhileLabel, funcName);
            dowhileLabel++;
        }
        else if(it->className == "PRINTF"){
            Printf *printfObject = static_cast<Printf*>( it->statementClass );
            printfObject->mipsPrintf(mipsFile, printfLabel, funcName);
            printfLabel++;
        }
        else if(it->className == "SCANF"){
            Scanf *scanfObject = static_cast<Scanf*>( it->statementClass );
            scanfObject->mipsScanf(mipsFile, scanfLabel, funcName);
            scanfLabel++;
        }
        else if(it->className == "EXIT"){
            Exit *exitObject = static_cast<Exit*>( it->statementClass );
            exitObject->mipsExit(mipsFile);
        }
        else if(it->className == "RETURN"){
            Return *returnObject = static_cast<Return*>( it->statementClass );
            returnObject->mipsReturn(mipsFile);
        }
        else if(it->className == "CALLFUNCTION"){
            CallFunction *callfunctionObject = static_cast<CallFunction*>( it->statementClass );
            callfunctionObject->mipsCallFunction(mipsFile, callfunctionLabel, funcName);
        }
        else if(it->className == "ASSIGNMENT"){
            Assignment *assignmentObject = static_cast<Assignment*>( it->statementClass );
            assignmentObject->mipsAssignment(mipsFile, assignmentLabel, funcName);
        }
        else {
            cout << "Estrutura não encontrada" << endl;
        }

    }
}

/*
*       MIPS PARA O CallFunction
*/
void CallFunction::mipsCallFunction(ofstream & mipsFile, int label, string funcName){
    vector<string> reg;
    int sp_size = this->params.size;
    //Fazer o minimal munch; Deve retornar um vetor com os registradores
    if(this->params.size <= 4) { //Colocar nos registradores $a
        if(this->params.size >= 1)
            mipsFile << "move $a0, " << reg[0]  << "\n";
        if(this->params.size >= 2)
            mipsFile << "move $a1, " << reg[1]  << "\n";
        if(this->params.size >= 3)
            mipsFile << "move $a2, " << reg[2]  << "\n";
        if(this->params.size >= 4)
            mipsFile << "move $a3, " << reg[3]  << "\n";
    } else {
        mipsFile << "addi $sp, $sp, -" << (sp_size*4) << "\n";
        for(int i = 0; i < sp_size; i++){
            mipsFile << "sw " << reg[i] << ", " << i << "($sp)"  << "\n";
        }
    }

    mipsFile << "jal " << this->funcName << "\n";
    mipsFile << "addi $sp, $sp, " << (sp_size*4) << "\n";
}

/*
*       MIPS PARA O Assignment
*/
void Assignment::mipsAssignment(ofstream & mipsFile, int label, string funcName){
    string reg;
    //Minimal Munch; Retorna o registrador em reg

    mipsFile << "move " << this->variable.name << ", " << reg << "\n";
}

/*
*       MIPS PARA O DoWhile
*/
void DoWhile::mipsDoWhile(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "DoWhileCondition" + label + funcName;
    string labelDoWhile = "DoWhile" + label + funcName;
    string labelExit = "DoWhileExit" + label + funcName;

    //Corpo do dowhile
    mipsFile << labelDoWhile << ":\n";
    for (std::list<Statement>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        it->mipsStatement(mipsFile, label+1, funcName);
    }

    //Condicao
    mipsFile << labelCondition << ":\n";
    this->condExp.mipsBOCBoolCondicional(mipsFile, labelDoWhile, labelExit);
    mipsFile << labelExit << ":\n";
}

/*
*       MIPS PARA O While
*/
void While::mipsWhile(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "WhileCondition" + label + funcName;
    string labelWhile = "While" + label + funcName;
    string labelExit = "WhileExit" + label + funcName;

    //Condicao
    mipsFile << labelCondition << ":\n";
    this->condExp.mipsBOCBoolCondicional(mipsFile, labelWhile, labelExit);

    //Corpo do while
    mipsFile << labelWhile << ":\n";
    for (std::list<Statement>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        it->mipsStatement(mipsFile, label+1, funcName);
    }

    mipsFile << "j " << labelCondition << "\n";
    mipsFile << labelExit << ":\n";
}

/*
*       MIPS PARA O BoolOperatorCondicional
*/
void BoolOperatorCondicional::mipsBOCAnd(ofstream & mipsFile, string op, string R1, string R2, string label) {
    if(op == "==") {
        mipsFile << "bnq " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "<=") {
        mipsFile << "bgt " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "<") {
        mipsFile << "bge " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == ">=") {
        mipsFile << "blt " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == ">") {
        mipsFile << "ble " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "!=") {
        mipsFile << "beq " << R1 << ", " << R2 << ", " << label <<"\n";
    }
}

void BoolOperatorCondicional::mipsBOCOR(ofstream & mipsFile, string op, string R1, string R2, string label) {
    if(op == "==") {
        mipsFile << "beq " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "<=") {
        mipsFile << "ble " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "<") {
        mipsFile << "blt " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == ">=") {
        mipsFile << "bge " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == ">") {
        mipsFile << "bgt " << R1 << ", " << R2 << ", " << label <<"\n";
    }
    else if(op == "!=") {
        mipsFile << "bnq " << R1 << ", " << R2 << ", " << label <<"\n";
    }
}

void BoolOperatorCondicional::mipsBOCBoolCondicional(ofstream & mipsFile, string labelThen, string labelElse){
    string R1, R2; //Registradores que o minimal munch tem que retornar
    BoolOperatorCondicional *aux = this;
    while(aux != NULL) {
        if(aux->boolOperator == "&&") {
            //Minimal munch para o lado esquerdo
            this->mipsBOCAnd(mipsFile, aux->condExpLeft->op, R1, R2, labelElse);
            if(aux->condExpRight != NULL){
                //Minimal munch para o lado direito
                this->mipsBOCAnd(mipsFile, aux->condExpRight->op, R1, R2, labelElse);
            }
        }
        else if(aux->boolOperator == "||") {
            //Minimal munch para o lado esquerdo
            this->mipsBOCOR(mipsFile, aux->condExpLeft->op, R1, R2, labelThen);
            
            if(aux->condExpRight != NULL){
                //Minimal munch para o lado direito
                this->mipsBOCAnd(mipsFile, aux->condExpRight->op, R1, R2, labelElse);
            }
        }
        else if(aux->boolOperator == "") {
            //Minimal munch para o lado esquerdo
            this->mipsBOCAnd(mipsFile, aux->condExpLeft->op, R1, R2, labelElse);
        }
        aux = aux->nextBOC;
    }
}

/*
*       MIPS PARA O IF
*/
void IF::mipsIF(ofstream & mipsFile, int label, string funcName){
    string labelThen = "ElseThen" + label + funcName;
    string labelElse = "ElseIF" + label + funcName;
    string labelEnd = "EndIF" + label  +  funcName;

    if(this->statementListElse.size > 0){
        //Condicional
        this->condExp.mipsBOCBoolCondicional(mipsFile, labelThen, labelElse);

        //corpo then
        mipsFile << labelThen << ":\n";
        for (std::list<Statement>::iterator it = this->statementListThen.begin(); it != this->statementListThen.end(); ++it) {
            it->mipsStatement(mipsFile, label+1, funcName);
        }
        mipsFile << "j" << labelEnd << "\n";

        //corpo else
        mipsFile << labelElse << ":\n";
        for (std::list<Statement>::iterator it = this->statementListElse.begin(); it != this->statementListElse.end(); ++it) {
            it->mipsStatement(mipsFile, label+2, funcName);
        }
    }
    else {
        //Condicional
        this->condExp.mipsBOCBoolCondicional(mipsFile, labelThen, labelEnd);
        //corpo then
        for (std::list<Statement>::iterator it = this->statementListThen.begin(); it != this->statementListThen.end(); ++it) {
            it->mipsStatement(mipsFile, label+1, funcName);
        }
    }

    mipsFile << labelEnd << ":\n";
}

/*
*       MIPS PARA O FOR
*/
void For::mipsFor(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "ForCondition" + label + funcName;
    string labelFor = "For" + label + funcName;
    string labelExit = "ForExit" + label + funcName;

    //Inicializacao
    //Minimal Munch para a inicializacao
    
    //Condicao de parada
    mipsFile << labelCondition << ":\n";
    this->condExp.mipsBOCBoolCondicional(mipsFile, labelFor, labelExit);
    
    mipsFile << labelFor << ":\n";
    
    //Corpo
    for (std::list<Statement>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        it->mipsStatement(mipsFile, label+1, funcName);
    }

    //Ajuste de valores
    //Minimal Munch para o ajuste de valores

    mipsFile << "j " << labelCondition << "\n";

    mipsFile << labelExit << ":\n"; 
}

/*
*       MIPS PARA O PRINTF
*/
void Printf::mipsPrintf(ofstream & mipsFile, int label, string funcName){
    string label = "Printf" + label + funcName;
    Variable *var;

    StringsLabel *ss = new StringsLabel();
    ss->label = label;
    ss->message = this->message;
    stringsLabelList.push_back(ss);
    mipsFile << "li $v0, 4\n";
    mipsFile << "la $a0, " << label;
    mipsFile << "syscall\n";

    for (std::list<ASTObject>::iterator it = this->paramsList.begin(); it != this->paramsList.end(); ++it) {
        // Minimal Munch; Retorna uma Variable
         
        if(var->type == "int") {
            mipsFile << "li $v0, 1\n";
            mipsFile << "la $a0, " << var->name <<"\n"; 
            mipsFile << "syscall\n";
        } else if(var->type == "float") {
            mipsFile << "li $v0, 2\n";
            mipsFile << "la $f2, " << var->name <<"\n";             
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->type == "double") {
            mipsFile << "li $v0, 3\n";
            mipsFile << "la $f2, " << var->name <<"\n";     
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->type == "char") {
            mipsFile << "li $v0, 4\n";
            mipsFile << "la $a0, " << var->name << "\n";
            mipsFile << "syscall\n";
        }
    }
}

/*
*       MIPS PARA O SCANF
*/
void Scanf::mipsScanf(ofstream & mipsFile, int label, string funcName){
    string label = "Scanf" + label + funcName;
    Variable *var;
    if(this->message != ""){
        StringsLabel *ss = new StringsLabel();
        ss->label = label;
        ss->message = this->message;
        stringsLabelList.push_back(ss);
        mipsFile << "li $v0, 4\n";
        mipsFile << "la $a0, " << label;
        mipsFile << "syscall\n";
    }

    for (std::list<Expression>::iterator it = this->variables.begin(); it != this->variables.end(); ++it) {
        // Minimal Munch; Retorna um Variable
        if(var->type == "int") {
            mipsFile << "li $v0, 5\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $v0" <<"\n"; 
        } else if(var->type == "float") {
            mipsFile << "li $v0, 6\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->type == "double") {
            mipsFile << "li $v0, 7\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->type == "char") {
            mipsFile << "li $a0, " << var->name << "\n";
            mipsFile << "li $a1, " << var->dimension[0] << "\n";
            mipsFile << "li $v0, 8\n";
            mipsFile << "syscall\n";
        }
    }
}

/*
*       MIPS PARA O EXIT
*/
void Exit::mipsExit(ofstream & mipsFile){
    string reg;
    //Chamar o minimal munch para a expressao
    //Colocar resultado da expressao em algum registrador
    
    mipsFile << "move $v0, " << reg << "\n";
    mipsFile << "syscall\n";
}

/*
*       MIPS PARA O RETURN
*/
void Return::mipsReturn(ofstream & mipsFile){
    string reg;
    //Chamar o minimal munch para a expressao
    //Colocar resultado da expressao em $vo para o retorno

    mipsFile << "move $v0, " << reg << "\n";
    mipsFile << "jr $ra\n";
}

/*
*       MIPS PARA O Expression
*/
// void printPostorder(Expression *ex) 
// { 
//     if (ex == NULL) 
//         return; 
  
//     // first recur on left subtree 
//     printPostorder(node->left); 
  
//     // then recur on right subtree 
//     printPostorder(node->right); 
  
//     // now deal with the node 
//     cout << node->data << " "; 
// } 

Variable *Expression::mipsMinimalMunch(ofstream & mipsFile, Expression *ex){
    if (ex == NULL) 
        return;
    ex->mipsMinimalMunch(mipsFile, ex->left);

    ex->mipsMinimalMunch(mipsFile, ex->right);
}

vector<string> Expression::mipsExpression(ofstream & mipsFile){
    Variable *reg1, *reg2;
    //Esquerda

    //Direita

    //Raiz
    if(reg1->type == "INT" && reg2->type == "INT"){
        if(this->op == "+") {
            mipsFile << "add mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "-") {
            mipsFile << "sub mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "*") {
            mipsFile << "mul " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "/") {
            mipsFile << "div " << reg1->name << ", " << reg2->name << "\n";
        }
    } 
    else if(reg1->type == "FLOAT" && reg2->type == "INT") {
        mipsFile << "mtc1 " << reg2->name << ", mips_aux_variable_con\n";
        mipsFile << "cvt.s.w mips_aux_variable_con, mips_aux_variable_con\n";
        if(this->op == "+") {
            mipsFile << "add.s mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "-") {
            mipsFile << "sub.s mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "*") {
            mipsFile << "mul.s mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "/") {
            mipsFile << "div.s mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        }
    } 
    else if(reg1->type == "INT" && reg2->type == "FLOAT") {
        mipsFile << "mtc1 " << reg1->name << ", mips_aux_variable_con\n";
        mipsFile << "cvt.s.w mips_aux_variable_con, mips_aux_variable_con\n";
        if(this->op == "+") {
            mipsFile << "add.s mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "-") {
            mipsFile << "sub.s mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "*") {
            mipsFile << "mul.s mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "/") {
            mipsFile << "div.s mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        }
    } 
    else if(reg1->type == "FLOAT" && reg2->type == "FLOAT") {
        if(this->op == "+") {
            mipsFile << "add.s mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "-") {
            mipsFile << "sub.s mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "*") {
            mipsFile << "mul.s " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "/") {
            mipsFile << "div.s " << reg1->name << ", " << reg2->name << "\n";
        }
    } 
    else if(reg1->type == "DOUBLE" && reg2->type == "INT") {
        mipsFile << "mtc1 " << reg2->name << ", mips_aux_variable_con\n";
        mipsFile << "cvt.s.w mips_aux_variable_con, mips_aux_variable_con\n";
        if(this->op == "+") {
            mipsFile << "add.d mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "-") {
            mipsFile << "sub.d mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "*") {
            mipsFile << "mul.d mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        } else if(this->op == "/") {
            mipsFile << "div.d mips_aux_variable, " << reg1->name << ", mips_aux_variable_con\n";
        }
    } 
    else if(reg1->type == "INT" && reg2->type == "DOUBLE") {
        mipsFile << "mtc1 " << reg1->name << ", mips_aux_variable_con\n";
        mipsFile << "cvt.d.w mips_aux_variable_con, mips_aux_variable_con\n";
        if(this->op == "+") {
            mipsFile << "add.d mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "-") {
            mipsFile << "sub.d mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "*") {
            mipsFile << "mul.d mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        } else if(this->op == "/") {
            mipsFile << "div.d mips_aux_variable, mips_aux_variable_con, " << reg2->name << "\n";
        }
    } 
    else if(reg1->type == "DOUBLE" || reg2->type == "DOUBLE") {
        if(this->op == "+") {
            mipsFile << "add.d mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "-") {
            mipsFile << "sub.d mips_aux_variable, " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "*") {
            mipsFile << "mul.d " << reg1->name << ", " << reg2->name << "\n";
        } else if(this->op == "/") {
            mipsFile << "div.d " << reg1->name << ", " << reg2->name << "\n";
        }
    }
}
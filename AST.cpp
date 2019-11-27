#include "AST.h"

int indexLabel = 0;
int floatLabel = 0, doubleLabel = 0;
string func_name;

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
        };
};


list<StringsLabel*> stringsLabelList;
list<DataLabel*> dataLabelList;

/*
*       MIPS PARA A Function
*/
void Function::mipsFunction(ofstream & mipsFile){
    mipsFile << this->name << ":\n";
    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        func_name = this->name;
        (*it)->mipsStatement(mipsFile, 0, this->name);
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
    
    for (std::list<ASTObject*>::iterator it = this->statement.begin(); it != this->statement.end(); ++it) {
        if((*it)->className == "IF"){
            IF *ifObject = static_cast<IF*>( (*it)->statementClass );
            ifObject->mipsIF(mipsFile, ifLabel, funcName);
            ifLabel++;
        }
        else if((*it)->className == "FOR"){
            For *forObject = static_cast<For*>( (*it)->statementClass );
            forObject->mipsFor(mipsFile, forLabel, funcName);
            forLabel++;
        }
        else if((*it)->className == "WHILE"){
            While *whileObject = static_cast<While*>( (*it)->statementClass );
            whileObject->mipsWhile(mipsFile, whileLabel, funcName);
            whileLabel++;
        }
        else if((*it)->className == "DOWHILE"){
            DoWhile *dowhileObject = static_cast<DoWhile*>( (*it)->statementClass );
            dowhileObject->mipsDoWhile(mipsFile, dowhileLabel, funcName);
            dowhileLabel++;
        }
        else if((*it)->className == "PRINTF"){
            Printf *printfObject = static_cast<Printf*>( (*it)->statementClass );
            printfObject->mipsPrintf(mipsFile, printfLabel, funcName);
            printfLabel++;
        }
        else if((*it)->className == "SCANF"){
            Scanf *scanfObject = static_cast<Scanf*>( (*it)->statementClass );
            scanfObject->mipsScanf(mipsFile, scanfLabel, funcName);
            scanfLabel++;
        }
        else if((*it)->className == "EXIT"){
            Exit *exitObject = static_cast<Exit*>( (*it)->statementClass );
            exitObject->mipsExit(mipsFile);
        }
        else if((*it)->className == "RETURN"){
            Return *returnObject = static_cast<Return*>( (*it)->statementClass );
            returnObject->mipsReturn(mipsFile);
        }
        else if((*it)->className == "CALLFUNCTION"){
            CallFunction *callfunctionObject = static_cast<CallFunction*>( (*it)->statementClass );
            callfunctionObject->mipsCallFunction(mipsFile, funcName);
        }
        else if((*it)->className == "ASSIGNMENT"){
            Assignment *assignmentObject = static_cast<Assignment*>( (*it)->statementClass );
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
void CallFunction::mipsCallFunction(ofstream & mipsFile, string funcName){
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
    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
    }

    //Condicao
    mipsFile << labelCondition << ":\n";
    this->condExp->mipsBOCBoolCondicional(mipsFile, labelDoWhile, labelExit);
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
    this->condExp->mipsBOCBoolCondicional(mipsFile, labelWhile, labelExit);

    //Corpo do while
    mipsFile << labelWhile << ":\n";
    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
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
        this->condExp->mipsBOCBoolCondicional(mipsFile, labelThen, labelElse);

        //corpo then
        mipsFile << labelThen << ":\n";
        for (std::list<Statement*>::iterator it = this->statementListThen.begin(); it != this->statementListThen.end(); ++it) {
            (*it)->mipsStatement(mipsFile, label+1, funcName);
        }
        mipsFile << "j" << labelEnd << "\n";

        //corpo else
        mipsFile << labelElse << ":\n";
        for (std::list<Statement*>::iterator it = this->statementListElse.begin(); it != this->statementListElse.end(); ++it) {
            (*it)->mipsStatement(mipsFile, label+2, funcName);
        }
    }
    else {
        //Condicional
        this->condExp->mipsBOCBoolCondicional(mipsFile, labelThen, labelEnd);
        //corpo then
        for (std::list<Statement*>::iterator it = this->statementListThen.begin(); it != this->statementListThen.end(); ++it) {
            (*it)->mipsStatement(mipsFile, label+1, funcName);
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
    this->condExp->mipsBOCBoolCondicional(mipsFile, labelFor, labelExit);
    
    mipsFile << labelFor << ":\n";
    
    //Corpo
    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
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

    for (std::list<ASTObject*>::iterator it = this->paramsList.begin(); it != this->paramsList.end(); ++it) {
        // Minimal Munch; Retorna uma Variable
         
        if(var->var_type == "int") {
            mipsFile << "li $v0, 1\n";
            mipsFile << "la $a0, " << var->name <<"\n"; 
            mipsFile << "syscall\n";
        } else if(var->var_type == "float") {
            mipsFile << "li $v0, 2\n";
            mipsFile << "la $f2, " << var->name <<"\n";             
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->var_type == "double") {
            mipsFile << "li $v0, 3\n";
            mipsFile << "la $f2, " << var->name <<"\n";     
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->var_type == "char") {
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

    for (std::list<Expression*>::iterator it = this->variables.begin(); it != this->variables.end(); ++it) {
        // Minimal Munch; Retorna um Variable
        if(var->var_type == "int") {
            mipsFile << "li $v0, 5\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $v0" <<"\n"; 
        } else if(var->var_type == "float") {
            mipsFile << "li $v0, 6\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->var_type == "double") {
            mipsFile << "li $v0, 7\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << var->name << ", $f0" <<"\n"; 
        } else if(var->var_type == "char") {
            mipsFile << "li $a0, " << var->name << "\n";
            mipsFile << "li $a1, " << var->dimension_size[0] << "\n";
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

Register mipsADDIConstant(ofstream & mipsFile, ASTObject *ast_obj){
    Register r;
    r.name = "";
    r.type = "";
    if(ast_obj->className == "NUMBER") {
        Number *aux = static_cast<Number*>(ast_obj->statementClass);
        if(aux->number_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->value << "\n";
            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->number_type == "FLOAT"){
            DataLabel *dt = new DataLabel();
            dt->value = aux->value;
            dt->data_type = "float";
            dt->label = "float" + floatLabel;
            dataLabelList.push_back(dt);
            mipsFile << "l.s F" << indexLabel << ", " << dt->label << "\n";
            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
            floatLabel++;
        }
        else if(aux->number_type == "DOUBLE"){
            DataLabel *dt = new DataLabel();
            dt->value = aux->value;
            dt->data_type = "double";
            dt->label = "double" + doubleLabel;
            dataLabelList.push_back(dt);
            mipsFile << "l.d F" << indexLabel << ", " << dt->label << "\n";
            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        } 
    }
    else if(ast_obj->className == "VARIABLE") {
        Variable *aux = static_cast<Variable*>(ast_obj->statementClass);
        if(aux->dimension_size.size > 0) return r;
        if(aux->var_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->name << "\n";
            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->var_type == "FLOAT"){
            mipsFile << "mov.s FS" << indexLabel << ", " << aux->name << "\n";
            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
            floatLabel++;
        }
        else if(aux->var_type == "DOUBLE"){
            mipsFile << "mov.d FD" << indexLabel << ", " << aux->name << "\n";
            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        }
    }
    else if(ast_obj->className == "CONSTANT") {
        Constant *aux = static_cast<Constant*>(ast_obj->statementClass);
        if(aux->const_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->value << "\n";
            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->const_type == "FLOAT"){
            DataLabel *dt = new DataLabel();
            dt->value = aux->value;
            dt->data_type = "float";
            dt->label = "float" + floatLabel;
            dataLabelList.push_back(dt);
            mipsFile << "l.s F" << indexLabel << ", " << dt->label << "\n";
            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
            floatLabel++;
        }
        else if(aux->const_type == "DOUBLE"){
            DataLabel *dt = new DataLabel();
            dt->value = aux->value;
            dt->data_type = "double";
            dt->label = "double" + doubleLabel;
            dataLabelList.push_back(dt);
            mipsFile << "l.d F" << indexLabel << ", " << dt->label << "\n";
            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        } 
    }
    else if(ast_obj->className == "CALLFUNCTION") {
        CallFunction *aux = static_cast<CallFunction*>(ast_obj->statementClass);
        
        if(aux->dimension_size.size > 0) return r;
        
        aux->mipsCallFunction(mipsFile, func_name);
        if(aux->callfunc_type == "INT") {
            mipsFile << "move R" << indexLabel << ", $zero, $v0\n";
            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->callfunc_type == "FLOAT") {
            mipsFile << "movs F" << indexLabel << ", $v0\n";
            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
        }
        else if(aux->callfunc_type == "DOUBLE") {
            mipsFile << "mov.d F" << indexLabel << ", $v0\n";
            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
        } 
    }

    indexLabel++;
    return r;
}

Register mipsADDIOperations(ofstream & mipsFile, string op, Register reg1, Register reg2){
    Register r;
    r.name = ""; r.type = "";
    if(reg1.type == "INT" && reg2.type == "INT"){
        if(op == "+") {
            mipsFile << "add R" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub R" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mult " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mflo R" << indexLabel << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mfhi R" << indexLabel << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "FLOAT" && reg2.type == "INT") {
        string auxr = reg2.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg2.name << ", " << auxr <<"\n";
        mipsFile << "cvt.s.w " << auxr << ", " << auxr << "\n";
        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "INT" && reg2.type == "FLOAT") {
        string auxr = reg1.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg1.name << ", " << auxr <<"\n";
        mipsFile << "cvt.s.w " << auxr << ", " << auxr << "\n";
        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "FLOAT" && reg2.type == "FLOAT") {
        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "DOUBLE" && reg2.type == "INT") {
        string auxr = reg2.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg2.name << ", " << auxr <<"\n";
        mipsFile << "cvt.d.w " << auxr << ", " << auxr << "\n";
        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "INT" && reg2.type == "DOUBLE") {
        string auxr = reg1.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg1.name << ", " << auxr <<"\n";
        mipsFile << "cvt.d.w " << auxr << ", " << auxr << "\n";
        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        }
    } 
    else if(reg1.type == "DOUBLE" || reg2.type == "DOUBLE") {
        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";
        }
    }

    indexLabel++;
    return r;
}

Register mipsLoad(ofstream & mipsFile, ASTObject *ast_obj, Register reg1) {
    Register r;
    r.name = ""; r.type = "";
    if(reg1.type == "INT") {
        if(ast_obj->className == "VARIABLE") {
            Variable *v = static_cast<Variable*>(ast_obj->statementClass);
            if(v->var_type == "INT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "INT"; r.tree = "LOAD";
            }
            if(v->var_type == "CHAR") {
                mipsFile << "addi aux, $zero, 1\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "LOAD";
            }
            if(v->var_type == "FLOAT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "l.s F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "LOAD";
            }
            if(v->var_type == "DOUBLE") {
                mipsFile << "addi aux, $zero, 8\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "l.d F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "LOAD";
            }
        }
        else if(ast_obj->className == "CALLFUNCTION") {
            CallFunction *c = static_cast<CallFunction*>(ast_obj->statementClass);
            if(c->callfunc_type == "INT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "INT"; r.tree = "LOAD";
            }
            if(c->callfunc_type == "CHAR") {
                mipsFile << "addi aux, $zero, 1\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "LOAD";
            }
            if(c->callfunc_type == "FLOAT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "l.s F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "LOAD";
            }
            if(c->callfunc_type == "DOUBLE") {
                mipsFile << "addi aux, $zero, 8\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "l.d F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "LOAD";
            }
        }
    }
    return r;
}

Register mipsStore(ofstream & mipsFile, ASTObject *ast_obj, Register reg1, Register reg2) {
    Register r;
    r.name = ""; r.type = "";

    if(reg1.type == reg2.type) {
        if(reg1.type == "INT") {
            mipsFile << "sw " << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";
        }
        if(reg1.type == "CHAR") {
            mipsFile << "sw " << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";
        }
        if(reg1.type == "FLOAT") {
            mipsFile << "s.s" << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";
        }
        if(reg1.type == "DOUBLE") {
            mipsFile << "s.d" << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";
        }
    }

    return r;
}

Register mipsMove(ofstream & mipsFile, Expression *ex, Register reg1, Register reg2) {
    Register r;
    r.name = ""; r.type = "";
    ASTObject *left = ex->left->term;
    ASTObject *right = ex->right->term;
    string regLeft, regRight;

    if(left->className == "VARIABLE") {
        Variable *v = static_cast<Variable*>(left->statementClass);
        regLeft = v->name;
    }

    if(right->className == "VARIABLE") {
        CallFunction *c = static_cast<CallFunction*>(right->statementClass);
        regRight = c->funcName;
    }

    if(reg1.type == reg2.type) {
        if(reg1.type == "INT") {
            mipsFile << "addi aux, $zero, 4\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "lw R" << indexLabel << ", (vetor2)\n";
            mipsFile << "sw R, (vetor1)\n";

            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "MOVE";
        }
        if(reg1.type == "CHAR") {
            mipsFile << "addi aux, $zero, 1\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "lw R" << indexLabel << ", (vetor2)\n";
            mipsFile << "sw R, (vetor1)\n";
            
            r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "MOVE";
        }
        if(reg1.type == "FLOAT") {
            mipsFile << "addi aux, $zero, 4\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "l.s R" << indexLabel << ", (vetor2)\n";
            mipsFile << "s.s R, (vetor1)\n";
            
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "MOVE";
        }
        if(reg1.type == "DOUBLE") {
            mipsFile << "addi aux, $zero, 4\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "l.d R" << indexLabel << ", (vetor2)\n";
            mipsFile << "s.d R, (vetor1)\n";
            
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "MOVE";
        }
    }

    return r;
}


Register Expression::mipsMinimalMunch(ofstream & mipsFile, Expression *ex){
    Register reg1, reg2, reg;
    reg.name = ""; reg.type = "";
    if (ex == NULL) 
        return reg;

    reg1 = ex->mipsMinimalMunch(mipsFile, ex->left);


    reg2 = ex->mipsMinimalMunch(mipsFile, ex->right);

    if(reg1.tree == "" && reg2.tree == "") {
        reg = mipsADDIConstant(mipsFile, ex->term);
    } 
    else if(reg1.tree == "CONSTANT" && reg2.tree == "CONSTANT") {
        reg = mipsADDIOperations(mipsFile, ex->op, reg1, reg2);
    }
    else if((reg1.tree == "CONSTANT" || reg1.tree == "OPERATION") && reg2.tree == "") { //Para vetores
        reg = mipsLoad(mipsFile, ex->term, reg1);
    }
    else if(reg1.name == "LOAD" && reg2.name != "") {
        reg = mipsStore(mipsFile, ex->term, reg1, reg2);
    }
    else if(reg1.name == "LOAD" && reg2.name == "LOAD") {
        reg = mipsMove(mipsFile, ex, reg1, reg2);
    }

    return reg;
}

vector<string> Expression::mipsExpression(ofstream & mipsFile){
    Variable *reg1, *reg2;
    //Esquerda
    this->mipsMinimalMunch(mipsFile, this->left);

    //Direita
    this->mipsMinimalMunch(mipsFile, this->right);

}
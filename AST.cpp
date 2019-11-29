#include "AST.h"

int indexLabel = 0;
int floatLabel = 0, doubleLabel = 0;
string func_name;
string currentLabel;

list<MipsInstruction*> instructionList;
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

    //Mips para a declaração de string para o scanf e printf
    for (std::list<StringsLabel*>::iterator it = stringsLabelList.begin(); it != stringsLabelList.end(); ++it) {
        (*it)->mipsStringsLabel(mipsFile);
    }

    //Mips para a declaração de floats e doubles
    for (std::list<DataLabel*>::iterator it = dataLabelList.begin(); it != dataLabelList.end(); ++it) {
        (*it)->mipsDataLabel(mipsFile);
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
        // else if((*it)->className == "ASSIGNMENT"){
        //     Assignment *assignmentObject = static_cast<Assignment*>( (*it)->statementClass );
        //     assignmentObject->mipsAssignment(mipsFile, assignmentLabel, funcName);
        // }
        else {
            cout << "Estrutura não encontrada" << endl;
        }

    }
}

/*
*       MIPS PARA O CallFunction
*/
void CallFunction::mipsCallFunction(ofstream & mipsFile, string funcName){
    Register reg;
    int sp_size = this->params.size();
    //Minimal Munch
    MipsInstruction *mips_inst;
    if(this->params.size() <= 4) { //Coloca nos registradores $a
        for(std::list<Expression*>::iterator it = this->params.begin(); it != this->params.end(); ++it){
            reg = (*it)->mipsExpression(mipsFile); //Minimal Munch
            if(this->params.size() >= 1){
                mipsFile << "move $a0, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a0", reg.name, "");
                instructionList.push_back(mips_inst);
            }
            if(this->params.size() >= 2){
                mipsFile << "move $a1, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a1", reg.name, "");
                instructionList.push_back(mips_inst);
            } 
            if(this->params.size() >= 3){
                mipsFile << "move $a2, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a2", reg.name, "");
                instructionList.push_back(mips_inst);
            }
            if(this->params.size() >= 4){
                mipsFile << "move $a3, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a3", reg.name, "");
                instructionList.push_back(mips_inst);
            }
        }
    } else {
        mipsFile << "addi $sp, $sp, -" << (sp_size*4) << "\n";
        mips_inst = new MipsInstruction(currentLabel, "addi", "$sp", "$sp", "-" + to_string(sp_size*4));
        instructionList.push_back(mips_inst);
        int i = 0;
        for(std::list<Expression*>::iterator it = this->params.begin(); it != this->params.end(); ++it){
            reg = (*it)->mipsExpression(mipsFile); //Minimal Munch
            mipsFile << "sw " << reg.name << ", " << i << "($sp)"  << "\n";
            mips_inst = new MipsInstruction(currentLabel, "sw", reg.name, to_string(i)+"($sp)", "");
            instructionList.push_back(mips_inst);
            i = i + 4;
        }
    }

    mipsFile << "jal " << this->funcName << "\n";
    mips_inst = new MipsInstruction(currentLabel, "jal", this->funcName, "", "");
    instructionList.push_back(mips_inst);

    mipsFile << "addi $sp, $sp, " << (sp_size*4) << "\n";
    mips_inst = new MipsInstruction(currentLabel, "addi", "$sp", "$sp", to_string(sp_size*4));
    instructionList.push_back(mips_inst);
}

/*
*       MIPS PARA O Assignment
*/
void Assignment::mipsAssignment(ofstream & mipsFile, int label, string funcName){
    Register reg;
    MipsInstruction *mips_inst;
    //Minimal Munch; Retorna o registrador em reg
    if(this->assignObject->className == "EXPRESSION") {
        Expression *ex = static_cast<Expression*>(this->assignObject->statementClass);
        reg = ex->mipsExpression(mipsFile);
    }
    else if(this->assignObject->className == "CALLFUNCTION") {
        CallFunction *c = static_cast<CallFunction*>(this->assignObject->statementClass);
        c->mipsCallFunction(mipsFile, func_name);
        reg.name = "$v0"; reg.type = c->callfunc_type;
    }

    mipsFile << "move " << this->variable.name << ", " << reg.name << "\n";
    mips_inst = new MipsInstruction(currentLabel, "move", this->variable.name, reg.name, "");
    instructionList.push_back(mips_inst);
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
    currentLabel = labelDoWhile;

    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
    }

    //Condicao
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;

    this->condExp->mipsBOCBoolCondicional(mipsFile, labelDoWhile, labelExit);
    mipsFile << labelExit << ":\n";
    currentLabel = labelExit;
}

/*
*       MIPS PARA O While
*/
void While::mipsWhile(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "WhileCondition" + label + funcName;
    string labelWhile = "While" + label + funcName;
    string labelExit = "WhileExit" + label + funcName;
    MipsInstruction *mips_inst;

    //Condicao
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;

    this->condExp->mipsBOCBoolCondicional(mipsFile, labelWhile, labelExit);

    //Corpo do while
    mipsFile << labelWhile << ":\n";
    currentLabel = labelWhile;

    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
    }

    mipsFile << "j " << labelCondition << "\n";
    mips_inst = new MipsInstruction(currentLabel, "j", labelCondition, "", "");
    instructionList.push_back(mips_inst);

    mipsFile << labelExit << ":\n";
    currentLabel = labelExit;
}

/*
*       MIPS PARA O BoolOperatorCondicional
*/
void BoolOperatorCondicional::mipsBOCAnd(ofstream & mipsFile, string op, string R1, string R2, string label) {
    MipsInstruction *mips_inst;
    if(op == "==") {
        mipsFile << "bnq " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bnq", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "<=") {
        mipsFile << "bgt " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bgt", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "<") {
        mipsFile << "bge " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bge", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == ">=") {
        mipsFile << "blt " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "blt", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == ">") {
        mipsFile << "ble " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "ble", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "!=") {
        mipsFile << "beq " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "beq", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
}

void BoolOperatorCondicional::mipsBOCOR(ofstream & mipsFile, string op, string R1, string R2, string label) {
    MipsInstruction *mips_inst;
    if(op == "==") {
        mipsFile << "beq " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "beq", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "<=") {
        mipsFile << "ble " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "ble", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "<") {
        mipsFile << "blt " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "blt", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == ">=") {
        mipsFile << "bge " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bge", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == ">") {
        mipsFile << "bgt " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bgt", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
    else if(op == "!=") {
        mipsFile << "bnq " << R1 << ", " << R2 << ", " << label <<"\n";
        mips_inst = new MipsInstruction(currentLabel, "bnq", R1, R2, label);
        instructionList.push_back(mips_inst);
    }
}

void BoolOperatorCondicional::mipsBOCBoolCondicional(ofstream & mipsFile, string labelThen, string labelElse){
    Register R1, R2;//Registradores que o minimal munch tem que retornar
    BoolOperatorCondicional *aux = this;
    while(aux != NULL) {
        if(aux->boolOperator == "&&") {
            //Minimal munch para o lado esquerdo
            R1 = this->condExpLeft->left->mipsExpression(mipsFile);
            R2 = this->condExpLeft->right->mipsExpression(mipsFile);
            this->mipsBOCAnd(mipsFile, aux->condExpLeft->op, R1.name, R2.name, labelElse);
            if(aux->condExpRight != NULL){
                //Minimal munch para o lado direito
                R1 = this->condExpRight->left->mipsExpression(mipsFile);
                R2 = this->condExpRight->right->mipsExpression(mipsFile);
                this->mipsBOCAnd(mipsFile, aux->condExpRight->op, R1.name, R2.name, labelElse);
            }
        }
        else if(aux->boolOperator == "||") {
            //Minimal munch para o lado esquerdo
            R1 = this->condExpLeft->left->mipsExpression(mipsFile);
            R2 = this->condExpLeft->right->mipsExpression(mipsFile);
            this->mipsBOCOR(mipsFile, aux->condExpLeft->op, R1.name, R2.name, labelThen);
            
            if(aux->condExpRight != NULL){
                //Minimal munch para o lado direito
                R1 = this->condExpRight->left->mipsExpression(mipsFile);
                R2 = this->condExpRight->right->mipsExpression(mipsFile);
                this->mipsBOCAnd(mipsFile, aux->condExpRight->op, R1.name, R2.name, labelElse);
            }
        }
        else if(aux->boolOperator == "") {
            //Minimal munch para o lado esquerdo
            R1 = this->condExpLeft->left->mipsExpression(mipsFile);
            R2 = this->condExpLeft->right->mipsExpression(mipsFile);
            this->mipsBOCAnd(mipsFile, aux->condExpLeft->op, R1.name, R2.name, labelElse);
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
    MipsInstruction *mips_inst;

    if(this->statementListElse.size() > 0){
        //Condicional
        this->condExp->mipsBOCBoolCondicional(mipsFile, labelThen, labelElse);

        //corpo then
        mipsFile << labelThen << ":\n";
        currentLabel = labelThen;
        for (std::list<Statement*>::iterator it = this->statementListThen.begin(); it != this->statementListThen.end(); ++it) {
            (*it)->mipsStatement(mipsFile, label+1, funcName);
        }

        mipsFile << "j" << labelEnd << "\n";
        mips_inst = new MipsInstruction(currentLabel, "j", labelEnd, "", "");
        instructionList.push_back(mips_inst);

        //corpo else
        mipsFile << labelElse << ":\n";
        currentLabel = labelElse;
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
    currentLabel = labelEnd;
}

/*
*       MIPS PARA O FOR
*/
void For::mipsFor(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "ForCondition" + label + funcName;
    string labelFor = "For" + label + funcName;
    string labelExit = "ForExit" + label + funcName;
    MipsInstruction *mips_inst;

    //Inicializacao
    //Minimal Munch para a inicializacao
    for (std::list<Expression*>::iterator it = this->valuesInitialization.begin(); it != this->valuesInitialization.end(); ++it) {
        (*it)->mipsExpression(mipsFile);
    }

    //Condicao de parada
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;
    this->condExp->mipsBOCBoolCondicional(mipsFile, labelFor, labelExit);
    
    mipsFile << labelFor << ":\n";
    currentLabel = labelFor;

    //Corpo
    for (std::list<Statement*>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        (*it)->mipsStatement(mipsFile, label+1, funcName);
    }

    //Ajuste de valores
    //Minimal Munch para o ajuste de valores
    for (std::list<Expression*>::iterator it = this->valuesAdjustment.begin(); it != this->valuesAdjustment.end(); ++it) {
        (*it)->mipsExpression(mipsFile);
    }

    mipsFile << "j " << labelCondition << "\n";
    mips_inst = new MipsInstruction(currentLabel, "j", labelCondition, "", "");
    instructionList.push_back(mips_inst);

    mipsFile << labelExit << ":\n";
    currentLabel = labelExit;
}

/*
*       MIPS PARA O PRINTF
*/
void Printf::mipsPrintf(ofstream & mipsFile, int label, string funcName){
    string labelA = "Printf" + label + funcName;
    Variable *var;
    Register reg;
    StringsLabel *ss = new StringsLabel();
    MipsInstruction *mips_inst;
    ss->label = labelA;
    ss->message = this->message;
    stringsLabelList.push_back(ss);

    mipsFile << "li $v0, 4\n";
    mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "4", "");
    instructionList.push_back(mips_inst);

    mipsFile << "la $a0, " << labelA;
    mips_inst = new MipsInstruction(currentLabel, "la", "$a0", labelA, "");
    instructionList.push_back(mips_inst);

    mipsFile << "syscall\n";
    mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
    instructionList.push_back(mips_inst);

    for (std::list<ASTObject*>::iterator it = this->paramsList.begin(); it != this->paramsList.end(); ++it) {
        //Minimal Munch
        if((*it)->className == "EXPRESSION") {
            Expression *ex = static_cast<Expression*>((*it)->statementClass);
            reg = ex->mipsExpression(mipsFile);
        }
        else if((*it)->className == "CALLFUNCTION") {
            CallFunction *c = static_cast<CallFunction*>((*it)->statementClass);
            c->mipsCallFunction(mipsFile, func_name);
            reg.name = "$v0"; reg.type = c->callfunc_type;
        }

        if(reg.type == "INT") {
            mipsFile << "li $v0, 1\n";
            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "1", "");
            instructionList.push_back(mips_inst);

            mipsFile << "la $a0, " << reg.name <<"\n";
            mips_inst = new MipsInstruction(currentLabel, "la", "$a0", reg.name, "");
            instructionList.push_back(mips_inst);

            mipsFile << "syscall\n";
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);
        } else if(reg.type == "FLOAT") {
            mipsFile << "li $v0, 2\n";
            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "2", "");
            instructionList.push_back(mips_inst); 

            mipsFile << "la $f12, " << reg.name <<"\n";
            mips_inst = new MipsInstruction(currentLabel, "la", "$f12", reg.name, "");
            instructionList.push_back(mips_inst); 

            mipsFile << "syscall\n";
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);

            mipsFile << "mov.s " << reg.name << ", $f0" <<"\n";
            mips_inst = new MipsInstruction(currentLabel, "mov.s", reg.name, "$f0", "");
            instructionList.push_back(mips_inst);

        } else if(reg.type == "DOUBLE") {
            mipsFile << "li $v0, 3\n";
            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "3", "");
            instructionList.push_back(mips_inst);

            mipsFile << "la $f12, " << reg.name <<"\n";   
            mips_inst = new MipsInstruction(currentLabel, "la", "$f12", reg.name, "");
            instructionList.push_back(mips_inst);

            mipsFile << "syscall\n";
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);

            mipsFile << "mov.d " << reg.name << ", $f0" <<"\n";
            mips_inst = new MipsInstruction(currentLabel, "mov.d ", reg.name, "$f0", "");
            instructionList.push_back(mips_inst);

        } else if(reg.type == "CHAR") {
            mipsFile << "li $v0, 4\n";
            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "4", "");
            instructionList.push_back(mips_inst);

            mipsFile << "la $a0, " << reg.name << "\n";
            mips_inst = new MipsInstruction(currentLabel, "la", "$a0", reg.name, "");
            instructionList.push_back(mips_inst);

            mipsFile << "syscall\n";
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);            
        }
    }
}

/*
*       MIPS PARA O SCANF
*/
void Scanf::mipsScanf(ofstream & mipsFile, int label, string funcName){
    string labelA = "Scanf" + label + funcName;
    Register reg;
    MipsInstruction *mips_inst;

    if(this->message != ""){
        StringsLabel *ss = new StringsLabel();
        ss->label = labelA;
        ss->message = this->message;
        stringsLabelList.push_back(ss);
        mipsFile << "li $v0, 4\n";
        mipsFile << "la $a0, " << label;
        mipsFile << "syscall\n";

        mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "4", "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "la", "$a0", labelA, "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
        instructionList.push_back(mips_inst);
    }

    for (std::list<Expression*>::iterator it = this->variables.begin(); it != this->variables.end(); ++it) {
        //Minimal Munch
        reg = (*it)->mipsExpression(mipsFile);
        if(reg.type == "INT") {
            mipsFile << "li $v0, 5\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << reg.name << ", $v0" <<"\n";

            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "5", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "move", reg.name,"$v0", "");
            instructionList.push_back(mips_inst);
        } else if(reg.type == "FLOAT") {
            mipsFile << "li $v0, 6\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << reg.name << ", $f0" <<"\n";

            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "6", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mov.s", reg.name,"$f0", "");
            instructionList.push_back(mips_inst);
        } else if(reg.type == "DOUBLE") {
            mipsFile << "li $v0, 7\n";
            mipsFile << "syscall\n";
            mipsFile << "move " << reg.name << ", $f0" <<"\n";

            mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "6", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mov.d", reg.name, "$f0", "");
            instructionList.push_back(mips_inst);
        } else if(reg.type == "char") {
            // mipsFile << "li $a0, " << reg.name << "\n";
            // mipsFile << "li $a1, " << var->dimension_size[0] << "\n";
            // mipsFile << "li $v0, 8\n";
            // mipsFile << "syscall\n";
        }
    }
}

/*
*       MIPS PARA O EXIT
*/
void Exit::mipsExit(ofstream & mipsFile){
    Register reg;
    MipsInstruction *mips_inst;
    //Chamar o minimal munch para a expressao
    reg = this->exp->mipsExpression(mipsFile);

    mipsFile << "move $v0, " << reg.name << "\n";
    mips_inst = new MipsInstruction(currentLabel, "move", "$v0", reg.name, "");
    instructionList.push_back(mips_inst);

    mipsFile << "syscall\n";
    mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
    instructionList.push_back(mips_inst);
}

/*
*       MIPS PARA O RETURN
*/
void Return::mipsReturn(ofstream & mipsFile){
    Register reg;
    MipsInstruction *mips_inst;
    //Chamar o minimal munch para a expressao
    reg = this->exp->mipsExpression(mipsFile);

    mipsFile << "move $v0, " << reg.name << "\n";
    mips_inst = new MipsInstruction(currentLabel, "move", "$v0", reg.name, "");
    instructionList.push_back(mips_inst);

    mipsFile << "jr $ra\n";
    mips_inst = new MipsInstruction(currentLabel, "jr", "$ra", "", "");
    instructionList.push_back(mips_inst);
}

/*
*       MIPS PARA O Expression
*/

Register mipsADDIConstant(ofstream & mipsFile, ASTObject *ast_obj){
    Register r;
    MipsInstruction *mips_inst;
    r.name = "";
    r.type = "";
    if(ast_obj->className == "NUMBER") {
        Number *aux = static_cast<Number*>(ast_obj->statementClass);
        if(aux->number_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->value << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->value);
            instructionList.push_back(mips_inst);

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
            mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), dt->label, "");
            instructionList.push_back(mips_inst);

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
            mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), dt->label, "");
            instructionList.push_back(mips_inst);

            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        } 
    }
    else if(ast_obj->className == "VARIABLE") {
        Variable *aux = static_cast<Variable*>(ast_obj->statementClass);
        if(aux->dimension_size.size() > 0) return r;
        if(aux->var_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->name << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->name);
            instructionList.push_back(mips_inst);

            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->var_type == "FLOAT"){
            mipsFile << "mov.s F" << indexLabel << ", " << aux->name << "\n";
            mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), aux->name, "");
            instructionList.push_back(mips_inst);

            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
            floatLabel++;
        }
        else if(aux->var_type == "DOUBLE"){
            mipsFile << "mov.d F" << indexLabel << ", " << aux->name << "\n";
            mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), aux->name, "");
            instructionList.push_back(mips_inst);


            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        }
    }
    else if(ast_obj->className == "CONSTANT") {
        Constant *aux = static_cast<Constant*>(ast_obj->statementClass);
        if(aux->const_type == "INT"){
            mipsFile << "addi R" << indexLabel << ", $zero, " << aux->value << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->value);
            instructionList.push_back(mips_inst);

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
            mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), dt->label, "");
            instructionList.push_back(mips_inst);

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
            mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), dt->label, "");
            instructionList.push_back(mips_inst);

            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
            doubleLabel++;
        } 
    }
    else if(ast_obj->className == "CALLFUNCTION") {
        CallFunction *aux = static_cast<CallFunction*>(ast_obj->statementClass);
        
        if(aux->dimension_size.size() > 0) return r;
        
        aux->mipsCallFunction(mipsFile, func_name);
        if(aux->callfunc_type == "INT") {
            mipsFile << "move R" << indexLabel << ", $v0\n";
            mips_inst = new MipsInstruction(currentLabel, "move", "R" + to_string(indexLabel), "$v0", "");
            instructionList.push_back(mips_inst);

            r.name = "R" + indexLabel;
            r.type = "INT"; r.tree = "CONSTANT";
        }
        else if(aux->callfunc_type == "FLOAT") {
            mipsFile << "mov.s F" << indexLabel << ", $v0\n";
            mips_inst = new MipsInstruction(currentLabel, "mov.s", "R" + to_string(indexLabel), "$v0", "");
            instructionList.push_back(mips_inst);

            r.name = "F" + indexLabel;
            r.type = "FLOAT"; r.tree = "CONSTANT";
        }
        else if(aux->callfunc_type == "DOUBLE") {
            mipsFile << "mov.d F" << indexLabel << ", $v0\n";
            mips_inst = new MipsInstruction(currentLabel, "mov.d", "F" + to_string(indexLabel), "$v0", "");
            instructionList.push_back(mips_inst);

            r.name = "F" + indexLabel;
            r.type = "DOUBLE"; r.tree = "CONSTANT";
        } 
    }

    indexLabel++;
    return r;
}

Register mipsADDIOperations(ofstream & mipsFile, string op, Register reg1, Register reg2){
    Register r;
    MipsInstruction *mips_inst;
    r.name = ""; r.type = "";
    if(reg1.type == "INT" && reg2.type == "INT"){
        if(op == "+") {
            mipsFile << "add R" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add", "R" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub R" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub", "R" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);        
        } else if(op == "*") {
            mipsFile << "mult " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mflo R" << indexLabel << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, reg2.name, "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mflo", "R" + to_string(indexLabel), "", "");
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mfhi R" << indexLabel << "\n";
            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div", reg1.name, reg2.name, "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mfhi", "R" + to_string(indexLabel), "", "");
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "FLOAT" && reg2.type == "INT") {
        string auxr = reg2.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg2.name << ", " << auxr <<"\n";
        mipsFile << "cvt.s.w " << auxr << ", " << auxr << "\n";

        mips_inst = new MipsInstruction(currentLabel, "mtc1", reg2.name, auxr, "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "cvt.s.w", auxr, auxr, "");
        instructionList.push_back(mips_inst);

        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.s", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.s", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.s", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div.s", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "INT" && reg2.type == "FLOAT") {
        string auxr = reg1.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg1.name << ", " << auxr <<"\n";
        mipsFile << "cvt.s.w " << auxr << ", " << auxr << "\n";

        mips_inst = new MipsInstruction(currentLabel, "mtc1", reg1.name, auxr, "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "cvt.s.w", auxr, auxr, "");
        instructionList.push_back(mips_inst);

        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.s", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.s", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.s", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div.s", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "FLOAT" && reg2.type == "FLOAT") {
        if(op == "+") {
            mipsFile << "add.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.s", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.s", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "*") {
            mipsFile << "mul.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.s", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div.s F" << indexLabel << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "OPERATION";
    
            mips_inst = new MipsInstruction(currentLabel, "div.s", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "DOUBLE" && reg2.type == "INT") {
        string auxr = reg2.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg2.name << ", " << auxr <<"\n";
        mipsFile << "cvt.d.w " << auxr << ", " << auxr << "\n";

        mips_inst = new MipsInstruction(currentLabel, "mtc1", reg2.name, auxr, "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "cvt.d.w", auxr, auxr, "");
        instructionList.push_back(mips_inst);

        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.d", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);            
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.d", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);            
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.d", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);            
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << reg1.name << ", " << auxr << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div.d", "F" + to_string(indexLabel), reg1.name, auxr);
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "INT" && reg2.type == "DOUBLE") {
        string auxr = reg1.name + to_string(indexLabel);
        mipsFile << "mtc1 " << reg1.name << ", " << auxr <<"\n";
        mipsFile << "cvt.d.w " << auxr << ", " << auxr << "\n";
        mips_inst = new MipsInstruction(currentLabel, "mtc1", reg1.name, auxr, "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "cvt.d.w", auxr, auxr, "");
        instructionList.push_back(mips_inst);

        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.d", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.d", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.d", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << auxr << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div.d", "F" + to_string(indexLabel), auxr, reg2.name);
            instructionList.push_back(mips_inst);
        }
    } 
    else if(reg1.type == "DOUBLE" || reg2.type == "DOUBLE") {
        if(op == "+") {
            mipsFile << "add.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add.d", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub.d", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "*") {
            mipsFile << "mul.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mul.d", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div.d F" << indexLabel  << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div.d", "F" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        }
    }

    indexLabel++;
    return r;
}

Register mipsLoad(ofstream & mipsFile, ASTObject *ast_obj, Register reg1) {
    Register r;
    MipsInstruction *mips_inst;
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

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", v->name, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
            }
            if(v->var_type == "CHAR") {
                mipsFile << "addi aux, $zero, 1\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "1");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", v->name, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
            }
            if(v->var_type == "FLOAT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "l.s F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", v->name, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
            }
            if(v->var_type == "DOUBLE") {
                mipsFile << "addi aux, $zero, 8\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << v->name << ", posic\n";
                mipsFile << "l.d F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "8");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", v->name, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
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

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", c->funcName, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
            }
            if(c->callfunc_type == "CHAR") {
                mipsFile << "addi aux, $zero, 1\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "lw R" << indexLabel << ", (vetor)\n";
                r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "1");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", c->funcName, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst);  
            }
            if(c->callfunc_type == "FLOAT") {
                mipsFile << "addi aux, $zero, 4\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "l.s F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", c->funcName, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst); 
            }
            if(c->callfunc_type == "DOUBLE") {
                mipsFile << "addi aux, $zero, 8\n";
                mipsFile << "mult " << reg1.name << ", aux\n";
                mipsFile << "mflo posic\n";
                mipsFile << "add vetor" << ", " << c->funcName << ", posic\n";
                mipsFile << "l.d F" << indexLabel << ", (vetor)\n";
                r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "LOAD";

                mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "8");
                instructionList.push_back(mips_inst);
                mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
                instructionList.push_back(mips_inst);  
                mips_inst = new MipsInstruction(currentLabel, "add", "vetor", c->funcName, "posic");
                instructionList.push_back(mips_inst); 
                mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), "(vetor)", "");
                instructionList.push_back(mips_inst); 
            }
        }
    }
    return r;
}

Register mipsStore(ofstream & mipsFile, ASTObject *ast_obj, Register reg1, Register reg2) {
    Register r;
    r.name = ""; r.type = "";
    MipsInstruction *mips_inst;

    if(reg1.type == reg2.type) {
        if(reg1.type == "INT") {
            mipsFile << "sw " << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";

            mips_inst = new MipsInstruction(currentLabel, "sw", reg2.name, "(" + reg1.name + ")", "");
            instructionList.push_back(mips_inst);
        }
        if(reg1.type == "CHAR") {
            mipsFile << "sw " << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";

            mips_inst = new MipsInstruction(currentLabel, "sw", reg2.name, "(" + reg1.name + ")", "");
            instructionList.push_back(mips_inst);
        }
        if(reg1.type == "FLOAT") {
            mipsFile << "s.s" << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";

            mips_inst = new MipsInstruction(currentLabel, "s.s", reg2.name, "(" + reg1.name + ")", "");
            instructionList.push_back(mips_inst);
        }
        if(reg1.type == "DOUBLE") {
            mipsFile << "s.d" << reg2.name << ", (" << reg1.name << ")\n";
            r = reg1; r.tree = "STORE";

            mips_inst = new MipsInstruction(currentLabel, "s.d", reg2.name, "(" + reg1.name + ")", "");
            instructionList.push_back(mips_inst);
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
    MipsInstruction *mips_inst;

    if(left->className == "VARIABLE") {
        Variable *v = static_cast<Variable*>(left->statementClass);
        regLeft = v->name;
    }

    if(right->className == "VARIABLE") {
        Variable *v = static_cast<Variable*>(right->statementClass);
        regRight = v->name;
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
            mipsFile << "sw R" << indexLabel << ", (vetor1)\n";

            r.name = "R" + indexLabel; r.type = "INT"; r.tree = "MOVE";

            mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor1", reg1.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "mult", reg2.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor2", reg2.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor2)", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "sw", "R" + to_string(indexLabel), "(vetor1)", "");
            instructionList.push_back(mips_inst); 
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
            mipsFile << "sw R" << indexLabel << ", (vetor1)\n";
            
            r.name = "R" + indexLabel; r.type = "CHAR"; r.tree = "MOVE";

            mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "1");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor1", reg1.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "mult", reg2.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor2", reg2.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(vetor2)", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "sw", "R" + to_string(indexLabel), "(vetor1)", "");
            instructionList.push_back(mips_inst); 
        }
        if(reg1.type == "FLOAT") {
            mipsFile << "addi aux, $zero, 4\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "l.s F" << indexLabel << ", (vetor2)\n";
            mipsFile << "s.s F" << indexLabel << ", (vetor1)\n";
            
            r.name = "F" + indexLabel; r.type = "FLOAT"; r.tree = "MOVE";

            mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor1", reg1.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "mult", reg2.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor2", reg2.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "l.s", "F" + to_string(indexLabel), "(vetor2)", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "s.s", "F" + to_string(indexLabel), "(vetor1)", "");
            instructionList.push_back(mips_inst); 
        }
        if(reg1.type == "DOUBLE") {
            mipsFile << "addi aux, $zero, 4\n";
            mipsFile << "mult " << reg1.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor1" << ", " << reg1.name << ", posic\n";

            mipsFile << "mult " << reg2.name << ", aux\n";
            mipsFile << "mflo posic\n";
            mipsFile << "add vetor2" << ", " << reg2.name << ", posic\n"; 
        
            mipsFile << "l.d F" << indexLabel << ", (vetor2)\n";
            mipsFile << "s.d F" << indexLabel << ", (vetor1)\n";
            
            r.name = "F" + indexLabel; r.type = "DOUBLE"; r.tree = "MOVE";

            mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "8");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor1", reg1.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "mult", reg2.name, "aux", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
            instructionList.push_back(mips_inst);  
            mips_inst = new MipsInstruction(currentLabel, "add", "vetor2", reg2.name, "posic");
            instructionList.push_back(mips_inst); 

            mips_inst = new MipsInstruction(currentLabel, "l.d", "F" + to_string(indexLabel), "(vetor2)", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "s.d", "F" + to_string(indexLabel), "(vetor1)", "");
            instructionList.push_back(mips_inst); 
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

Register Expression::mipsExpression(ofstream & mipsFile){
    Register reg1, reg2, reg;
    //Esquerda
    reg1 = this->mipsMinimalMunch(mipsFile, this->left);

    //Direita
    reg2 = this->mipsMinimalMunch(mipsFile, this->right);

    if(reg1.tree == "" && reg2.tree == "") {
        reg = mipsADDIConstant(mipsFile, this->term);
    } 
    else if(reg1.tree == "CONSTANT" && reg2.tree == "CONSTANT") {
        reg = mipsADDIOperations(mipsFile, this->op, reg1, reg2);
    }
    else if((reg1.tree == "CONSTANT" || reg1.tree == "OPERATION") && reg2.tree == "") { //Para vetores
        reg = mipsLoad(mipsFile, this->term, reg1);
    }
    else if(reg1.name == "LOAD" && reg2.name != "") {
        reg = mipsStore(mipsFile, this->term, reg1, reg2);
    }
    else if(reg1.name == "LOAD" && reg2.name == "LOAD") {
        reg = mipsMove(mipsFile, this, reg1, reg2);
    }
    return reg;
}

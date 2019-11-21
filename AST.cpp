#include "AST.h"

void Function::mipsFunction(ofstream & mipsFile){
    mipsFile << this->name << ":\n";
    for (std::list<Statement>::iterator it = this->statementList.begin(); it != this->statementList.end(); ++it) {
        it->mipsStatement(mipsFile, 0, this->name);
    }

}

void Statement::mipsStatement(ofstream & mipsFile, int label, string funcName){
    int ifLabel = label,forLabel = label, whileLabel = label, dowhileLabel = label;
    
    for (std::list<ObjectStatement>::iterator it = this->statement.begin(); it != this->statement.end(); ++it) {
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
            printfObject->mipsPrintf(mipsFile);
        }
        else if(it->className == "SCANF"){
            Scanf *scanfObject = static_cast<Scanf*>( it->statementClass );
            scanfObject->mipsScanf(mipsFile);
        }
        else if(it->className == "EXIT"){
            Exit *exitObject = static_cast<Exit*>( it->statementClass );
            exitObject->mipsExit(mipsFile);
        }
        else if(it->className == "RETURN"){
            Return *returnObject = static_cast<Return*>( it->statementClass );
            returnObject->mipsReturn(mipsFile);
        }
        else {
            cout << "Estrutura não encontrada" << endl;
        }

    }
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





void Printf::mipsPrintf(ofstream & mipsFile){

}


void Scanf::mipsScanf(ofstream & mipsFile){

}

void Exit::mipsExit(ofstream & mipsFile){
    //Chamar o minimal munch para a expressao
    //Colocar resultado da expressao em algum registrador
    //chamar a função exit
}

void Return::mipsReturn(ofstream & mipsFile){
    //Chamar o minimal munch para a expressao
    //Colocar resultado da expressao em $vo para o retorno
    mipsFile << "jr $ra\n";
}
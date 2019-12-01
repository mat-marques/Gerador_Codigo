#include "AST.h"

int indexLabel = 0, vectorLabel = 0;
int printLabel = 0;
string func_name;
string currentLabel;

list<MipsInstruction*> instructionList;
list<StringsLabel*> stringsLabelList;
list<DataLabel*> dataLabelList;

Register mipsStore(ofstream & mipsFile, Register reg1, Register reg2);
Register mipsMove(ofstream & mipsFile, Register reg1, Register reg2);

/*
*       MIPS PARA A Program
*/
void Program::mipsProgram(ofstream & mipsFile){
	MipsInstruction *mips_inst;

	currentLabel = "begin";
	mipsFile << "j main\n";
	mips_inst = new MipsInstruction(currentLabel, "j", "main", "", "");
	instructionList.push_back(mips_inst);

    for (std::list<Function*>::iterator it = this->functionsList.begin(); it != this->functionsList.end(); ++it) {
        mipsFile << (*it)->name << ":\n";
        currentLabel = (*it)->name;
        func_name = (*it)->name;

        if((*it)->name != "main"){
			mipsFile << "addi $sp, $sp, -" << (13 * 4) << "\n";

			mips_inst = new MipsInstruction(currentLabel, "addi", "$sp", "$sp", "-"+to_string(13*4));
			instructionList.push_back(mips_inst);

			for(int i = 0; i < 13; i++) {
				if(i<=7){
					mipsFile << "sw  $t" << i << ", "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$t"+to_string(i), to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
				else if(i==8){
					mipsFile << "sw  $a0, "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$a0", to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
				else if(i==9){
					mipsFile << "sw  $a1, "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$a1", to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
				else if(i==10){
					mipsFile << "sw  $a2, "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$a2", to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
				else if(i==11){
					mipsFile << "sw  $a3, "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$a3", to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
				else {
					mipsFile << "sw  $ra, "<< (i * 4) << "($sp)\n";
					mips_inst = new MipsInstruction(currentLabel, "sw", "$ra", to_string((i * 4)) + "($sp)", "");
					instructionList.push_back(mips_inst);
				}
			}
        }

    	(*it)->mipsFunction(mipsFile);

    }

    //Mips para a declaração de string para o scanf e printf
    for (std::list<StringsLabel*>::iterator it = stringsLabelList.begin(); it != stringsLabelList.end(); ++it) {
        (*it)->mipsStringsLabel(mipsFile);
    }

    //Mips para a declaração de floats e doubles
//    for (std::list<DataLabel*>::iterator it = dataLabelList.begin(); it != dataLabelList.end(); ++it) {
//        (*it)->mipsDataLabel(mipsFile);
//    }
}


void Program::mips(){
	string label;
    //Mips para a declaração de string para o scanf e printf
    for (std::list<StringsLabel*>::iterator it = stringsLabelList.begin(); it != stringsLabelList.end(); ++it) {
        (*it)->mips();
    }

    label = "";
    for (std::list<MipsInstruction*>::iterator it = instructionList.begin(); it != instructionList.end(); ++it) {
    	if((*it)->label != label){
        	cout << (*it)->label << ":\n";
        	label = (*it)->label;
    	}
    	(*it)->mips();
    }

}



/*
*       MIPS PARA A Function
*/
void Function::mipsFunction(ofstream & mipsFile){
   this->statementList->mipsStatement(mipsFile, 0, this->name);
}

/*
*       MIPS PARA O Statement
*/
void Statement::mipsStatement(ofstream & mipsFile, int label, string funcName){
    int ifLabel = label,forLabel = label, whileLabel = label, dowhileLabel = label, assignmentLabel = label,
    printfLabel = label, scanfLabel = label;
    
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
//        else {
//            cout << "Estrutura não encontrada: " << (*it)->className << endl;
//        }

    }
}

/*
*       MIPS PARA um VETOR
*/
Register Variable::mipsVariableVector(ofstream & mipsFile, string funcName){
	Register reg;
	vector<Register> aux_reg;
	MipsInstruction *mips_inst;

	for (std::vector<Expression*>::iterator it = this->dimension_exp.begin(); it != this->dimension_exp.end(); ++it) {
		Register r = (*it)->mipsExpression(mipsFile);
		aux_reg.push_back(r);
	}

	if(this->dimension_size.size() == 1){
	    mipsFile << "addi aux, $zero, 4\n";
	    mipsFile << "mult " << aux_reg[0].name << ", aux\n";
	    mipsFile << "mflo posic\n";
	    mipsFile << "add vetor_" << this->name << vectorLabel << ", " << this->name << ", posic\n";

        mips_inst = new MipsInstruction(currentLabel, "addi", "aux", "$zero", "4");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "mult", aux_reg[0].name, "aux", "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "mflo", "posic", "", "");
        instructionList.push_back(mips_inst);
        mips_inst = new MipsInstruction(currentLabel, "add", "vetor_"+this->name+ to_string(vectorLabel), this->name , "posic");
        instructionList.push_back(mips_inst);

        reg.name = "R" + to_string(indexLabel); reg.type = "INT"; reg.vetor = "vetor_"+this->name+ to_string(vectorLabel);  reg.tree = "VECTOR";
        vectorLabel++;
	}

	return reg;
}

/*
*       MIPS PARA O CallFunction
*/
Register CallFunction::mipsCallFunction(ofstream & mipsFile, string funcName){
    Register reg;
    int sp_size = this->params.size(), i = 1;
    //Minimal Munch
    MipsInstruction *mips_inst;
    if(this->params.size() <= 4) { //Coloca nos registradores $a
        for(std::list<Expression*>::iterator it = this->params.begin(); it != this->params.end(); ++it){
            reg = (*it)->mipsExpression(mipsFile); //Minimal Munch
            if(i == 1){
                mipsFile << "move $a0, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a0", reg.name, "");
                instructionList.push_back(mips_inst);
            }
            if(i == 2){
                mipsFile << "move $a1, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a1", reg.name, "");
                instructionList.push_back(mips_inst);
            } 
            if(i == 3){
                mipsFile << "move $a2, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a2", reg.name, "");
                instructionList.push_back(mips_inst);
            }
            if(i == 4){
                mipsFile << "move $a3, " << reg.name  << "\n";
                mips_inst = new MipsInstruction(currentLabel, "move", "$a3", reg.name, "");
                instructionList.push_back(mips_inst);
            }
            i++;
        }

        mipsFile << "jal " << this->funcName << "\n";
        mips_inst = new MipsInstruction(currentLabel, "jal", this->funcName, "", "");
        instructionList.push_back(mips_inst);

        mipsFile << "move R" << indexLabel  << ", $v0\n";
        mips_inst = new MipsInstruction(currentLabel, "move", "R"+to_string(indexLabel), "$v0", "");
        instructionList.push_back(mips_inst);

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
        mipsFile << "jal " << this->funcName << "\n";

        mips_inst = new MipsInstruction(currentLabel, "jal", this->funcName, "", "");
        instructionList.push_back(mips_inst);

        mipsFile << "move R" << indexLabel  << ", $v0\n";
        mips_inst = new MipsInstruction(currentLabel, "move", "R"+to_string(indexLabel), "$v0", "");
        instructionList.push_back(mips_inst);

        mipsFile << "addi $sp, $sp, " << (sp_size*4) << "\n";
        mips_inst = new MipsInstruction(currentLabel, "addi", "$sp", "$sp", to_string(sp_size*4));
        instructionList.push_back(mips_inst);
    }

    reg.name = "R"+to_string(indexLabel); reg.type = "INT";
    if(this->dimension_size.size() == 1) {
    	reg.tree = "VECTOR";
    }

    indexLabel++;
    return reg;
}

/*
*       MIPS PARA O Assignment
*/
void Assignment::mipsAssignment(ofstream & mipsFile, int label, string funcName){
    Register reg, reg2;
    MipsInstruction *mips_inst;
    //Minimal Munch; Retorna o registrador em reg
    if(this->assignObject->className == "EXPRESSION") {
        Expression *ex = static_cast<Expression*>(this->assignObject->statementClass);
        reg = ex->mipsExpression(mipsFile);
    }
    else if(this->assignObject->className == "CALLFUNCTION") {
        CallFunction *c = static_cast<CallFunction*>(this->assignObject->statementClass);
        reg = c->mipsCallFunction(mipsFile, func_name);
    }

    if(this->variable->dimension_size.size() == 1) { //Atribuição que usa vetores
    	if(reg.tree == "VECTOR") { //Move
    		reg2 = this->variable->mipsVariableVector(mipsFile, funcName);
    		reg = mipsMove(mipsFile, reg2, reg);
    	} else { //Store
    		reg2 = this->variable->mipsVariableVector(mipsFile, funcName);
    		reg = mipsStore(mipsFile, reg2, reg);
    	}
    }
    else {
		mipsFile << "move " << this->variable->name << ", " << reg.name << "\n";
		mips_inst = new MipsInstruction(currentLabel, "move", this->variable->name, reg.name, "");
		instructionList.push_back(mips_inst);
    }
}

/*
*       MIPS PARA O DoWhile
*/
void DoWhile::mipsDoWhile(ofstream & mipsFile, int label, string funcName){
	MipsInstruction *mips_inst;
    string labelCondition = "DoWhileCondition" + to_string(label) + funcName;
    string labelDoWhile = "DoWhile" + to_string(label) + funcName;
    string labelExit = "DoWhileExit" + to_string(label) + funcName;

    //Corpo do dowhile
    mipsFile << labelDoWhile << ":\n";
    currentLabel = labelDoWhile;

    this->statementList->mipsStatement(mipsFile, label+1, funcName);

    //Condicao
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;

    this->condExp->mipsBOCBoolCondicional(mipsFile, labelDoWhile, labelExit);
    mipsFile << "j " << labelDoWhile << "\n";
	mips_inst = new MipsInstruction(currentLabel, "j", labelDoWhile, "", "");
	instructionList.push_back(mips_inst);

    mipsFile << labelExit << ":\n";

    currentLabel = labelExit;
}

/*
*       MIPS PARA O While
*/
void While::mipsWhile(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "WhileCondition" + to_string(label) + funcName;
    string labelWhile = "While" + to_string(label) + funcName;
    string labelExit = "WhileExit" + to_string(label) + funcName;
    MipsInstruction *mips_inst;

    //Condicao
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;

    this->condExp->mipsBOCBoolCondicional(mipsFile, labelWhile, labelExit);

    //Corpo do while
    mipsFile << labelWhile << ":\n";
    currentLabel = labelWhile;

    this->statementList->mipsStatement(mipsFile, label+1, funcName);


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
    string labelThen = "ThenIF" + to_string(label) + funcName;
    string labelElse = "ElseIF" + to_string(label) + funcName;
    string labelEnd = "EndIF" + to_string(label) +  funcName;
    MipsInstruction *mips_inst;

    if(this->statementListElse != NULL){
        //Condicional
        this->condExp->mipsBOCBoolCondicional(mipsFile, labelThen, labelElse);

        //corpo then
        mipsFile << labelThen << ":\n";
        currentLabel = labelThen;

        this->statementListThen->mipsStatement(mipsFile, label+1, funcName);


        mipsFile << "j " << labelEnd << "\n";
        mips_inst = new MipsInstruction(currentLabel, "j", labelEnd, "", "");
        instructionList.push_back(mips_inst);

        //corpo else
        mipsFile << labelElse << ":\n";
        currentLabel = labelElse;

        this->statementListElse->mipsStatement(mipsFile, label+2, funcName);
    }
    else {
        //Condicional
        this->condExp->mipsBOCBoolCondicional(mipsFile, labelThen, labelEnd);
        //corpo then
        mipsFile << labelThen << ":\n";
        currentLabel = labelThen;
        this->statementListThen->mipsStatement(mipsFile, label+1, funcName);
    }

    mipsFile << labelEnd << ":\n";
    currentLabel = labelEnd;
}

/*
*       MIPS PARA O FOR
*/
void For::mipsFor(ofstream & mipsFile, int label, string funcName){
    string labelCondition = "ForCondition" + to_string(label) + funcName;
    string labelFor = "For" + to_string(label) + funcName;
    string labelExit = "ForExit" + to_string(label) + funcName;
    MipsInstruction *mips_inst;

    //Inicializacao
    //Minimal Munch para a inicializacao
    for (std::list<Assignment*>::iterator it = this->valuesInitialization.begin(); it != this->valuesInitialization.end(); ++it) {
        (*it)->mipsAssignment(mipsFile, label, funcName);
    }

    //Condicao de parada
    mipsFile << labelCondition << ":\n";
    currentLabel = labelCondition;
    this->condExp->mipsBOCBoolCondicional(mipsFile, labelFor, labelExit);
    
    mipsFile << labelFor << ":\n";
    currentLabel = labelFor;

    //Corpo
    this->statementList->mipsStatement(mipsFile, label+1, funcName);


    //Ajuste de valores
    //Minimal Munch para o ajuste de valores
    for (std::list<Assignment*>::iterator it = this->valuesAdjustment.begin(); it != this->valuesAdjustment.end(); ++it) {
    	(*it)->mipsAssignment(mipsFile, label, funcName);
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
    string labelA;
    Register reg;
    StringsLabel *ss;
    MipsInstruction *mips_inst;
    int i, j = 0;
    std::list<ASTObject*>::iterator it = this->paramsList.begin();

    for(i = 0; i < this->message.size(); i++) {
    	if(this->message[i] != "%d"){
			ss = new StringsLabel();
			labelA = "Printf" + to_string(printLabel + i) + funcName;
			ss->label = labelA;
			ss->message = this->message[i];
			stringsLabelList.push_back(ss);
    	}
    }

    for(i = 0; i < this->message.size(); i++) {
    	if(this->message[i] != "%d"){
			mipsFile << "li $v0, 4\n";
			mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "4", "");
			instructionList.push_back(mips_inst);

			labelA = "Printf" + to_string(printLabel+i) + funcName;
			mipsFile << "la $a0, " << labelA <<"\n";
			mips_inst = new MipsInstruction(currentLabel, "la", "$a0", labelA, "");
			instructionList.push_back(mips_inst);

			mipsFile << "syscall\n";
			mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
			instructionList.push_back(mips_inst);

    	}
    	else {
			//Minimal Munch
			if((*it)->className == "EXPRESSION") {
				Expression *ex = static_cast<Expression*>((*it)->statementClass);
				reg = ex->mipsExpression(mipsFile);
			}
			else if((*it)->className == "CALLFUNCTION") {
				CallFunction *c = static_cast<CallFunction*>((*it)->statementClass);
				reg = c->mipsCallFunction(mipsFile, func_name);
			}

			if(reg.type == "INT") {
				mipsFile << "li $v0, 1\n";
				mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "1", "");
				instructionList.push_back(mips_inst);

				mipsFile << "move $a0, " << reg.name <<"\n";
				mips_inst = new MipsInstruction(currentLabel, "move", "$a0", reg.name, "");
				instructionList.push_back(mips_inst);

				mipsFile << "syscall\n";
				mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
				instructionList.push_back(mips_inst);
			}
    		it++;
			j++;
    	}
    }

    printLabel = printLabel + i;
}

/*
*       MIPS PARA O SCANF
*/
void Scanf::mipsScanf(ofstream & mipsFile, int label, string funcName){
    string labelA;
	Register reg;
	StringsLabel *ss;
	MipsInstruction *mips_inst;

	for (std::list<Expression*>::iterator it = this->paramsList.begin(); it != this->paramsList.end(); ++it) {
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

    if(func_name != "main"){
		for(int i = 0; i < 13; i++) {
			if(i<=7){
				mipsFile << "lw  $t" << i << ", "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$t"+to_string(i), to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
			else if(i==8){
				mipsFile << "lw  $a0, "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$a0", to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
			else if(i==9){
				mipsFile << "lw  $a1, "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$a1", to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
			else if(i==10){
				mipsFile << "lw  $a2, "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$a2", to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
			else if(i==11){
				mipsFile << "lw  $a3, "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$a3", to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
			else {
				mipsFile << "lw  $ra, "<< (i * 4) << "($sp)\n";
				mips_inst = new MipsInstruction(currentLabel, "lw", "$ra", to_string((i * 4)) + "($sp)", "");
				instructionList.push_back(mips_inst);
			}
		}

		mipsFile << "addi $sp, $sp, " << (13 * 4) << "\n";
		mips_inst = new MipsInstruction(currentLabel, "addi", "$sp", "$sp", to_string(13*4));
		instructionList.push_back(mips_inst);

		mipsFile << "jr $ra\n";
		mips_inst = new MipsInstruction(currentLabel, "jr", "$ra", "", "");
		instructionList.push_back(mips_inst);
    } else {
    	mipsFile << "li $v0, 10\n";
    	mipsFile << "syscall\n";
		mips_inst = new MipsInstruction(currentLabel, "li", "$v0", "10", "");
		instructionList.push_back(mips_inst);
		mips_inst = new MipsInstruction(currentLabel, "syscall", "", "", "");
		instructionList.push_back(mips_inst);
    }
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
            mipsFile << "addi R" << to_string(indexLabel) << ", $zero, " << aux->value << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->value);
            instructionList.push_back(mips_inst);

            r.name = "R" + to_string(indexLabel);
            r.type = "INT"; r.tree = "CONSTANT";
        }
    }
    else if(ast_obj->className == "VARIABLE") {
        Variable *aux = static_cast<Variable*>(ast_obj->statementClass);
        if(aux->dimension_size.size() > 0){
        	r = aux->mipsVariableVector(mipsFile, func_name);
        	return r;
        };
        if(aux->var_type == "INT"){
            mipsFile << "addi R" << to_string(indexLabel) << ", $zero, " << aux->name << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->name);
            instructionList.push_back(mips_inst);

            r.name = "R" + to_string(indexLabel);
            r.type = "INT"; r.tree = "CONSTANT";
        }
    }
    else if(ast_obj->className == "CONSTANT") {
        Constant *aux = static_cast<Constant*>(ast_obj->statementClass);
        if(aux->const_type == "INT"){
            mipsFile << "addi R" << to_string(indexLabel) << ", $zero, " << aux->value << "\n";
            mips_inst = new MipsInstruction(currentLabel, "addi", "R" + to_string(indexLabel), "$zero", aux->value);
            instructionList.push_back(mips_inst);

            r.name = "R" + to_string(indexLabel);
            r.type = "INT"; r.tree = "CONSTANT";
        }
    }
    else if(ast_obj->className == "CALLFUNCTION") {
        CallFunction *aux = static_cast<CallFunction*>(ast_obj->statementClass);
        
        r = aux->mipsCallFunction(mipsFile, func_name);
        if(aux->callfunc_type == "INT") {
            r.tree = "CONSTANT";
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
            mipsFile << "add R" << to_string(indexLabel) << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "add", "R" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);
        } else if(op == "-") {
            mipsFile << "sub R" << to_string(indexLabel) << ", " << reg1.name << ", " << reg2.name << "\n";
            r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "sub", "R" + to_string(indexLabel), reg1.name, reg2.name);
            instructionList.push_back(mips_inst);        
        } else if(op == "*") {
            mipsFile << "mult " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mflo R" << to_string(indexLabel) << "\n";
            r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "mult", reg1.name, reg2.name, "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mflo", "R" + to_string(indexLabel), "", "");
            instructionList.push_back(mips_inst);
        } else if(op == "/") {
            mipsFile << "div " << reg1.name << ", " << reg2.name << "\n";
            mipsFile << "mfhi R" << to_string(indexLabel) << "\n";
            r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "OPERATION";

            mips_inst = new MipsInstruction(currentLabel, "div", reg1.name, reg2.name, "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "mfhi", "R" + to_string(indexLabel), "", "");
            instructionList.push_back(mips_inst);
        }
    } 

    indexLabel++;
    return r;
}

Register mipsLoad(ofstream & mipsFile,Register reg1) {
    Register r;
    MipsInstruction *mips_inst;
    r.name = ""; r.type = "";
    if(reg1.type == "INT") {
		mipsFile << "lw R" << to_string(indexLabel) << ", (" << reg1.vetor << ")\n";
		r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "LOAD";

		mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(" + reg1.vetor + ")", "");
		instructionList.push_back(mips_inst);
    }

    indexLabel++;
    return r;
}

Register mipsStore(ofstream & mipsFile, Register reg1, Register reg2) {
    Register r;
    r.name = ""; r.type = "";
    MipsInstruction *mips_inst;
    cout << reg1.type << " " << reg2.type << endl;
    if(reg1.type == reg2.type) {
        if(reg1.type == "INT") {
            mipsFile << "sw " << reg2.name << ", (" << reg1.vetor << ")\n";
            r = reg1; r.tree = "STORE";

            mips_inst = new MipsInstruction(currentLabel, "sw", reg2.name, "(" + reg1.vetor + ")", "");
            instructionList.push_back(mips_inst);
        }
    }

    return r;
}

Register mipsMove(ofstream & mipsFile, Register reg1, Register reg2) {
    Register r;
    r.name = ""; r.type = "";
    string regLeft, regRight;
    MipsInstruction *mips_inst;

    if(reg1.type == reg2.type) {
        if(reg1.type == "INT") {

            mipsFile << "lw R" << to_string(indexLabel) << ", (" << reg2.vetor << ")\n";
            mipsFile << "sw R" << to_string(indexLabel) << ", (" << reg1.vetor << ")\n";

            r.name = "R" + to_string(indexLabel); r.type = "INT"; r.tree = "MOVE";

            mips_inst = new MipsInstruction(currentLabel, "lw", "R" + to_string(indexLabel), "(" + reg2.vetor + ")", "");
            instructionList.push_back(mips_inst);
            mips_inst = new MipsInstruction(currentLabel, "sw", "R" + to_string(indexLabel), "(" + reg1.vetor + ")", "");
            instructionList.push_back(mips_inst); 
        }
    }

    indexLabel++;
    return r;
}

Register Expression::mipsMinimalMunch(ofstream & mipsFile, Expression *ex){
    Register reg1, reg2, reg;
    reg.name = ""; reg.type = "";
    if (ex == NULL) 
        return reg;

    reg1 = ex->mipsMinimalMunch(mipsFile,ex->left);
    if(reg1.tree == "VECTOR") {
        reg1 = mipsLoad(mipsFile, reg1);
    }

    reg2 = ex->mipsMinimalMunch(mipsFile, ex->right);
	if(reg2.tree == "VECTOR") {
		reg2 = mipsLoad(mipsFile, reg2);;
	}

    if(reg1.tree == "" && reg2.tree == "") {
        reg = mipsADDIConstant(mipsFile, ex->term);
    } 
    else if((reg1.tree == "CONSTANT" && reg2.tree == "CONSTANT") ||
    		(reg1.tree == "CONSTANT" && reg2.tree == "OPERATION" ) ||
			(reg1.tree == "CONSTANT" && reg2.tree == "LOAD" ) ||
			(reg1.tree == "LOAD" && reg2.tree == "CONSTANT" ) ||
    		(reg1.tree == "LOAD" && reg2.tree == "OPERATION") ||
			(reg1.tree == "LOAD" && reg2.tree == "LOAD"))
    {
        reg = mipsADDIOperations(mipsFile, ex->op, reg1, reg2);
    }

    return reg;
}

Register Expression::mipsExpression(ofstream & mipsFile){
    Register reg1, reg2, reg;
    //Esquerda
    reg1 = this->mipsMinimalMunch(mipsFile, this->left);
    if(reg1.tree == "VECTOR") {
    	reg1 = mipsLoad(mipsFile, reg1);
    }

    //Direita
    reg2 = this->mipsMinimalMunch(mipsFile, this->right);
    if(reg2.tree == "VECTOR") {
    	reg2 = mipsLoad(mipsFile, reg2);
    }

    if(reg1.tree == "" && reg2.tree == "") {
        reg = mipsADDIConstant(mipsFile, this->term);
    } 
    else if((reg1.tree == "CONSTANT" && reg2.tree == "CONSTANT") ||
    		(reg1.tree == "CONSTANT" && reg2.tree == "OPERATION" ) ||
			(reg1.tree == "CONSTANT" && reg2.tree == "LOAD" ) ||
			(reg1.tree == "LOAD" && reg2.tree == "CONSTANT" ) ||
    		(reg1.tree == "LOAD" && reg2.tree == "OPERATION") ||
			(reg1.tree == "LOAD" && reg2.tree == "LOAD"))
    {
        reg = mipsADDIOperations(mipsFile, this->op, reg1, reg2);
    }

    return reg;
}

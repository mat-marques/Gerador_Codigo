#include "Arquivo.h"
#include "AST.h"

Statement *bodyStatements(string input);
BoolOperatorCondicional *condicionalParser(string line);

Arquivo::Arquivo(vector<string> file_input){
    this->readFile(file_input);
}

vector<string> splitText(string text, string c) {
	vector<string> word;
    size_t pos = 0;
    string token;
    while ((pos = text.find(c)) != std::string::npos) {
        token = text.substr(0, pos);
        word.push_back(token);
        text.erase(0, pos + c.length());
    }
    word.push_back(text);
	return word;
}


int get_argumentIndex(char c, int argSize, int startIndex, string argument)
{
    int i;
	for(i = startIndex; i < argSize; i++)
	{
		if(argument[i] == c)
			return i;
	}
	return -1;
}

int get_argumentIndexRev(char c, int startIndex, string argument)
{
    int i;
	for(i = startIndex; i >= 0; i--)
	{
		if(argument[i] == c)
			return i;
	}
	return -1;
}

string strtoupper(string str)
{
    int leng=str.length();
    for(int i=0; i<leng; i++)
        if (97<=str[i]&&str[i]<=122)//a-z
            str[i]-=32;
    return str;
}

Printf *printParser(string line){
	Printf *printf_ = new Printf();

    int n = line.size();
    int i, j;
    string aux;
    /* Provavelmente tenho que arrumar algo nessa busca pelo ',' */
    i = get_argumentIndex(',', line.size(), 0, line);
    if(line[i-1] != '"'){
        i = get_argumentIndex('"', line.size(), 0, line);
        j = get_argumentIndex('"', line.size(), i+1, line);
        j = j-i-1;
        aux = line.substr(i+1, j);
        cout << "IMPRIMINDO SEM VARIAVEIS: " << aux << endl;
        
    }
    else{
        i = get_argumentIndex('"', line.size(), 0, line);
        j = get_argumentIndex('"', line.size(), i+1, line);
        aux = line.substr(i, j);
        /*Verificar tipo da variavel */
        i = 0; 
        while(i != -1){
            i = get_argumentIndex('%', aux.size(), i, aux);
            if(i != -1){
                if(aux[i+1] == 'd'){
                    cout << "IMPRIMINDO INT" << endl;
                }
                if(aux[i+1] == 'c'){
                    cout << "IMPRIMINDO CHAR" << endl;
                }
                if(aux[i+1] == 'f'){
                    cout << "IMPRIMINDO FLOAT" << endl;
                }
                if(aux[i+1] == 's'){
                    cout << "IMPRIMINDO STRING" << endl;
                }
                i++;
            }    
        }
        aux.clear();
        i = get_argumentIndex(',', line.size(), j+1, line);
        if(line[line.size() == ';']){
            j = line.size() -2;
        }
        else{
            j = line.size()-1;
        }
        j = j-i-1;
        aux = line.substr(i+1, j);
        cout << "VARIAVEIS A SEREM PRINTADAS: " << aux << endl;
    }
    return printf_;
}

Scanf *scanfParser(string line){
	Scanf *scanf_ = new Scanf();

    int n = line.size();
    int i, j;
    string aux;
    i = get_argumentIndex(',', line.size(), 0, line);
    if(line[i-1] == '"'){
        i = get_argumentIndex('"', line.size(), 0, line);
        j = get_argumentIndex('"', line.size(), i+1, line);
        aux = line.substr(i, j);
        /*Verificar tipo da variavel */
        i = 0; 
        while(i != -1){
            i = get_argumentIndex('%', aux.size(), i, aux);
            if(i != -1){
                if(aux[i+1] == 'd'){
                    cout << "LER INT" << endl;
                }
                if(aux[i+1] == 'c'){
                    cout << "LER CHAR" << endl;
                }
                if(aux[i+1] == 'f'){
                    cout << "LER FLOAT" << endl;
                }
                if(aux[i+1] == 's'){
                    cout << "LER STRING" << endl;
                }
                i++;
            }    
        }
        aux.clear();
        i = get_argumentIndex(',', line.size(), j+1, line);
        j = get_argumentIndex('(', line.size(), i+1, line);        
        i = get_argumentIndex(')', line.size(), j+1, line);
        i = i-j-1;
        aux = line.substr(j+1, i);
        cout << "Lendo variavel: " << aux << endl;
    }

    return scanf_;
}

ASTObject* verifyExpression(string line, int s){
	ASTObject *ex = new Expression();
    int n = line.size();
    int i;
    int j;
    string token;
    string aux1, aux2;
    string result;
    string conc;
    vector<string> parser;
    cout << line << endl;
    for(i = n; i >= 0; i--){
        /*binarios*/
        if(line[i] == '+'){
            if(line[i-1] != '+'){
                if(line[i+1] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(1, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " + " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
            if(line[i+1] == '='){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " += " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '-'){
            if(line[i-1] != '-'){
                if(line[i+1] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(1, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " - " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
            if(line[i+1] == '='){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " -= " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '*'){
            if(line[i+1] == '('){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " * " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
        }
        if(line[i] == '/'){
            if(line[i+1] == '('){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " / " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
        }
        if(line[i] == '%'){
            if(line[i+1] == '('){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " % " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
        }
        if(line[i] == '&'){
            if(line[i+1] == '(' && line[i-1] != '&'){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " & " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
            if(line[i-1] == '&'){
                if(line[i+1] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(1, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i-1);
                        result = conc + aux1 + " && " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '|'){
            if(line[i+1] == '(' && line[i-1] != '|'){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " | " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
            if(line[i-1] == '|'){
                if(line[i+1] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(1, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i-1);
                        result = conc + aux1 + " || " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '^'){
            if(line[i+1] == '('){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " ^ " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
        }
        
        if(line[i] == '='){
            if(line[i+1] == '(' && line[i-1] != '<' && line[i-1] != '>' && line[i-1] != '+' && line[i-1] != '-'){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " = " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
            if(line[i-1] == '='){
                if(line[i+1] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);;
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " == " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '!'){
            if(line[i+1] == '='){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " != " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '<'){
            if(line[i+1] == '(' && line[i-1] != '<'){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " < " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
            if(line[i+1] == '='){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " <= " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
            if(line[i+1] == '<'){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " << " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        if(line[i] == '>'){
            if(line[i+1] == '(' && line[i-1] != '>'){
                j = get_argumentIndexRev(')', line.size(), line);
                if(j != -1){
                    j = j-i-1;
                    token = line.substr(i+1, j);
                    parser = splitText(token, ",");
                    aux1 = parser[0].substr(1, parser[0].size());
                    aux2 = parser[1].substr(0, parser[1].size());
                    conc = line.substr(0, i);
                    result = conc + aux1 + " > " + aux2;
                    cout << result << endl;
                    return result;
                }          
            }
            if(line[i+1] == '='){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " >= " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
            if(line[i+1] == '>'){
                if(line[i+2] == '('){
                    j = get_argumentIndexRev(')', line.size(), line);
                    if(j != -1){
                        j = j-i-1;
                        token = line.substr(i+1, j);
                        parser = splitText(token, ",");
                        aux1 = parser[0].substr(2, parser[0].size());
                        aux2 = parser[1].substr(0, parser[1].size());
                        conc = line.substr(0, i);
                        result = conc + aux1 + " >> " + aux2;
                        cout << result << endl;
                        return result;
                    }          
                }
            }
        }
        /*unarios*/
        if(line[i] == '+'){
            if(line[i-1] == '+'){        
                if(line[i+1] == '('){   
                    j = get_argumentIndex(')', line.size(), i+2, line);
                    if(j != -1){
                        j = j - i - 2;
                        cout << i << endl;
                        token = line.substr(i+2, j);
                        result = token + " = " + token + " + " + "1";
                        cout << result << endl;
                        return result;
                    }
                }
            }
            if(line[i-1] == '+'){
                if(line[i-2] == ')'){    
                    j = get_argumentIndexRev('(', line.size(), line);
                    if(j != -1){
                        i = i-j-3;
                        token = line.substr(j+1, i);
                        result = token + " = " + token + " + " + "1";
                        cout << result << endl;
                        return result;
                    }
                }
            }
        }
        if(line[i] == '-'){
            if(line[i-1] == '-'){        
                if(line[i+1] == '('){   
                    j = get_argumentIndex(')', line.size(), i+2, line);
                    if(j != -1){
                        j = j - i - 2;
                        cout << i << endl;
                        token = line.substr(i+2, j);
                        result = token + " = " + token + " - " + "1";
                        cout << result << endl;
                        return result;
                    }
                }
            }
            if(line[i-1] == '-'){
                if(line[i-2] == ')'){    
                    j = get_argumentIndexRev('(', line.size(), line);
                    if(j != -1){
                        i = i-j-3;
                        token = line.substr(j+1, i);
                        result = token + " = " + token + " - " + "1";
                        cout << result << endl;
                        return result;
                    }
                }
            }
        }
        if(line[i] == '*'){
           
        }
        if(line[i] == '+'){
            if(line[i+1] == '+'){

            }
        }
        if(line[i] == '-'){
            if(line[i+1] == '-'){

            }
        }
        if(line[i] == '~'){
            
        }
        if(line[i] == '!'){
            
        }
        if(line[i] == '&'){
            
        }

    }
    return ex;
}

IF* ifParser(string line){
	IF *if_ = new IF();

    int n = line.size();
    int i;
    string result;
    cout << line << endl;
    for(i = 0; i < n; i++){
        if(line[i] == '<' || line[i] == '+' || line[i] == '-' || line[i] == '=' || line[i] == '*' || line[i] == '>'
         || line[i] =='^' || line[i] == '/' || line[i] =='%' || line[i] =='&' || line[i] =='|' || line[i] =='!'){
            result = verifyExpression(line, 0);
            while(result.compare("Pronto") != 0){
                result = verifyExpression(result, 0);
                
            }
        } 
    }

    return if_;
}

Return* returnParser(string line){
	Return *return_ = new Return();
	ASTObject *ex;

    ex = verifyExpression(line, 0);

    return_->exp = static_cast<Expression*>(ex->statementClass);
    return return_ ;
}

Exit* exitParser(string line){
	Exit *exit_ = new Exit();
	ASTObject *ex;

    ex = verifyExpression(line, 0);

    exit_->exp = static_cast<Expression*>(ex->statementClass);
    return exit_;
}

string splitExpression(string line) {
    int i = 0;
    string value = "";
    int openB = 0, closeB = 0;
    while(true) {
        if((openB == closeB && line[i] == ',') || (i == line.size())){
            break;
        }
        if(line[i] == '(') openB++;
        if(openB > closeB && line[i] == ')') closeB++;
        value = value + line[i];
        i++;
    }

    return value;
}

string isolatedExpression(string line) {
    int i = 0;
    string value = "";
    while(line[i] != ',') {
        value = value + line[i];
        i++;
    }
    value = value + ')';
    return value;
}

CondicionalExpression *mathOperator(string line) {
	CondicionalExpression *cond_ex = new CondicionalExpression();
	ASTObject *ex;
    string aux, ex_l, ex_r;
    int i = 0;
    if(line[i] == '(') i++;

    if(line[i] == '=' && line[i+1] == '='){
        aux = line.substr(i+2, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "==";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);

    } else if(line[i] == '!' && line[i+1] == '=') {
        aux = line.substr(i+2, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "!=";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);
    } else if(line[i] == '<' && line[i+1] == '=') {
        aux = line.substr(i+2, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "<=";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);
    } else if(line[i] == '>' && line[i+1] == '=') {
        aux = line.substr(i+2, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = ">=";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);

    } else if(line[i] == '<') {
        aux = line.substr(i+1, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "<";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);

    } else if(line[i] == '>') {
        aux = line.substr(i+1, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = ">";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

        //Direita
        ex_r = aux.substr(ex_l.size()+2, aux.size());
        ex_r = splitExpression(ex_r);
        aux = '(' + ex_r;
        ex_r = aux;
        ex = verifyExpression(ex_r, 0);

        cond_ex->right = static_cast<Expression*>(ex->statementClass);

    } else if(line[i] == '!') {
        aux = line.substr(i+1, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "!";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);

    } else if(line[i] == '*') {
        aux = line.substr(i+1, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "*";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);
    } else if(line[i] == '&') {
        aux = line.substr(i+1, line.size());
        aux = splitExpression(aux);
        cout << aux << endl;

        cond_ex->op = "&";

        //Esquerda
        ex_l = aux.substr(1, aux.size());
        ex_l = splitExpression(ex_l);
        ex = verifyExpression(ex_l, 0);

        cond_ex->left = static_cast<Expression*>(ex->statementClass);
    }

    return cond_ex;
}

BoolOperatorCondicional *logicalOperator(string line) {
    string aux, ex_l,ex_r;
    int i = 0;
    BoolOperatorCondicional *bool_op = NULL;

    if(line[i] == '(') i++;

    if(line[i] == '&' && line[i+1] == '&'){
    	bool_op = new BoolOperatorCondicional();
		bool_op->boolOperator = "&&";
    	BoolOperatorCondicional *bool_op_aux = NULL;
    	aux = line.substr(i+2, line.size()-1);
		cout << aux << endl;

		//Esquerda
		ex_l = aux.substr(1, aux.size());
		ex_l = splitExpression(ex_l);
		bool_op_aux = condicionalParser(ex_l);

		if(bool_op_aux != NULL) {
			bool_op->condExpLeft = bool_op_aux->condExpLeft;
		}

		bool_op_aux = NULL;

		//Direita
		ex_r = line.substr(ex_l.size()+5, line.size());
		ex_r = splitExpression(ex_r);
		ex_r = ex_r.substr(0, ex_r.size()-1);
		bool_op_aux = condicionalParser(ex_r);

		if(bool_op_aux != NULL) {
			bool_op->condExpRight = bool_op_aux->condExpLeft;
		}


    } else if(line[i] == '|' && line[i+1] == '|') {
    	bool_op = new BoolOperatorCondicional();
		bool_op->boolOperator = "&&";

    	BoolOperatorCondicional *bool_op_aux = NULL;
    	aux = line.substr(i+2, line.size()-1);

		//Esquerda
		ex_l = aux.substr(1, aux.size());
		ex_l = splitExpression(ex_l);
		bool_op_aux = condicionalParser(ex_l);

		if(bool_op_aux != NULL) {
			bool_op->condExpLeft = bool_op_aux->condExpLeft;
		}

		bool_op_aux = NULL;

		//Direita
		ex_r = aux.substr(ex_l.size()+5, line.size());
		ex_r = splitExpression(ex_r);
		ex_r = ex_r.substr(0, ex_r.size()-1);
		bool_op_aux = condicionalParser(ex_r);

		if(bool_op_aux != NULL) {
			bool_op->condExpRight = bool_op_aux->condExpLeft;
		}

    }

    return bool_op;
}


BoolOperatorCondicional *condicionalParser(string line) {
    int i = 0;
    string aux;
    BoolOperatorCondicional *bool_op = NULL;
    ASTObject *ex;
    CondicionalExpression *con_ex;

    if(line[i] == '(') i++;

    if((line[i] == '&' && line[i+1] == '&')|| (line[i] == '|' || line[i+1] == '|')){
    	bool_op = logicalOperator(line);
    }
    else if((line[i] == '=' && line[i+1] == '=') || (line[i] == '!' && line[i+1] == '=') ||
    (line[i] == '<' && line[i+1] == '=') || (line[i] == '>' && line[i+1] == '=') ||
    (line[i] == '<') || (line[i] == '>') || (line[i] == '!') || (line[i] == '*') ||
    (line[i] == '&')) {
       bool_op = new BoolOperatorCondicional();
       con_ex = mathOperator(line);
       bool_op->condExpLeft = con_ex;
    }
    else {
    	bool_op = new BoolOperatorCondicional();
    	con_ex = new CondicionalExpression();
        aux = isolatedExpression(line);
        ex = verifyExpression(aux, 0);

        con_ex->left = static_cast<Expression*>(ex->statementClass);
        bool_op->condExpLeft = con_ex;

    }

    return bool_op;
}


Assignment *initValues(string line){
	Assignment *assign = new Assignment();
    int i = 0;
    string aux = "";
    ASTObject *ex;
    Expression *ex_l;
    if(line[i] == '(') i++;

    if(line[i] == '='){
    	aux = splitExpression(line.substr(i, line.size()));
    	ex = verifyExpression(aux, 0);
    	assign->assignObject = ex;
    	ex_l = static_cast<Expression*>(ex);
    	assign->variable = static_cast<Variable*>(ex_l->term);
    }
    else {
        aux = isolatedExpression(line.substr(i, line.size()));
        ex = verifyExpression(aux, 0);
        assign->assignObject = ex;
    }

    return assign;
}

Assignment *adjustValues(string line){
	Assignment *assign = new Assignment();
    int i = 0;
    string aux = "";
    ASTObject *ex;
    Expression *ex_l;
    if(line[i] == '(') i++;

    if(line[i] == '='){
    	aux = splitExpression(line.substr(i, line.size()));
    	ex = verifyExpression(aux, 0);
    	assign->assignObject = ex;
    	ex_l = static_cast<Expression*>(ex);
    	assign->variable = static_cast<Variable*>(ex_l->term);
    }
    else {
        aux = isolatedExpression(line.substr(i, line.size()));
        ex = verifyExpression(aux, 0);
        assign->assignObject = ex;
    }

    return assign;
}

While* whileParser(string line) {
	While *while_ = new While();
    Statement *statement;
	BoolOperatorCondicional *bool_op;
    string aux;

    bool_op = condicionalParser(line);
    while_->condExp = bool_op;

    aux = splitExpression(line.substr(1, line.size()-2));
    aux = line.substr(aux.size()+2, line.size());
    aux = aux.substr(0, aux.size()-2);
    statement = bodyStatements(aux);
    while_->statementList = statement;

    return while_;
}

DoWhile *dowhileParser(string line) {
	DoWhile *dowhile = new DoWhile();
    string aux;


    return dowhile;
}

For *forParser(string line) {
	For *for_ = new For();
	BoolOperatorCondicional *bool_op;
	Assignment *assig1, *assig2;
    string aux, aux2;
    Statement *statement;
    int i = 0;

    aux = splitExpression(line.substr(1, line.size()));
    aux = line.substr(aux.size()+2, line.size());
    aux = aux.substr(0, aux.size()-1);
    i = line.size() - aux.size() - 1;
    assig1 = initValues(line);
    for_->valuesInitialization.push_back(assig1);

    aux = splitExpression(line.substr(i, line.size()));
    aux = line.substr(aux.size()+1, line.size());
    aux = aux.substr(0, aux.size()-1);
    i = i + line.size() - aux.size() - 1;
    bool_op = condicionalParser(aux);
    for_->condExp = bool_op;


    aux = line.substr(i, line.size());
    aux = aux.substr(0, aux.size()-1);
    assig2 = adjustValues(aux);
    for_->valuesAdjustment.push_back(assig2);

    aux2 = splitExpression(aux);
    aux = aux.substr(aux2.size()+1, aux.size());
    statement = bodyStatements(aux);
    for_->statementList = statement;

    return for_;
}


string splitStatement(string line) {
    int i = 0;
    string value = "";
    int openB = 0, closeB = 0;
    while(true) {
        if((openB == closeB && line[i] == ';') || (i == line.size())){
            break;
        }
        if(line[i] == '(') openB++;
        if(line[i] == ')') closeB++;
        value = value + line[i];
        i++;
    }

    return value;
}


Statement *bodyStatements(string input) {
    string aux;
    string line = input;
    ASTObject *ast_obj;
    IF *if_;
    For *for_;
    While *while_;
    DoWhile *dowhile_;
    Printf *printf_;
    Scanf *scanf_;
    Exit *exit_;
    Return *return_;
    Assignment *assign;
    Statement *statement = new Statement();

    while(true){
    	if(line.size() >= 2) aux = line.substr(0, 2);
        if(aux.compare("IF") == 0){
            cout << "COMANDO IF: " << endl;
            aux = splitStatement(line);
            ifParser(aux);

            ast_obj->statementClass = static_cast<void*>(if_);
			ast_obj->className = "IF";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 6) aux = line.substr(0, 6);
        if(aux.compare("PRINTF") == 0){
            cout << "COMANDO PRINTF: " << endl;
            aux = splitStatement(line);
            printParser(aux);

            ast_obj->statementClass = static_cast<void*>(printf_);
			ast_obj->className = "PRINTF";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 5) aux = line.substr(0, 5);
        if(aux.compare("SCANF") == 0){
            cout << "COMANDO SCANF:" << endl;
            aux = splitStatement(line);
            scanfParser(aux);

            ast_obj->statementClass = static_cast<void*>(scanf_);
			ast_obj->className = "SCANF";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 6) aux = line.substr(0, 6);
        if(aux.compare("RETURN") == 0){
            cout << "COMANDO RETURN:" << endl;
            aux = splitStatement(line);
            cout << aux << endl;
            returnParser(aux);

            ast_obj->statementClass = static_cast<void*>(return_);
			ast_obj->className = "RETURN";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 5) aux = line.substr(0, 5);
        if(aux.compare("WHILE") == 0){
            cout << "COMANDO WHILE:" << endl;
            aux = splitStatement(line);
            whileParser(aux);

            ast_obj->statementClass = static_cast<void*>(while_);
			ast_obj->className = "WHILE";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 3) aux = line.substr(0, 3);
        if(aux.compare("FOR") == 0){
            cout << "COMANDO FOR:" << endl;
            aux = splitStatement(line);

            ast_obj->statementClass = static_cast<void*>(for_);
			ast_obj->className = "FOR";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 4) aux = line.substr(0, 4);
        if(aux.compare("EXIT") == 0){
            cout << "COMANDO EXIT:" << endl;
            aux = splitStatement(line);
            exitParser(aux);

            ast_obj->statementClass = static_cast<void*>(exit_);
			ast_obj->className = "EXIT";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 7) aux = line.substr(0, 7);
        if(aux.compare("DO_WHILE") == 0){
            cout << "COMANDO DO_WHILE:" << endl;
            aux = splitStatement(line);

            ast_obj->statementClass = static_cast<void*>(dowhile_);
			ast_obj->className = "DOWHILE";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }

        if(line.size() >= 1) aux = line.substr(0, 1);
        if(aux.compare("=") == 0){
            cout << "COMANDO ASSIGNMENT:" << endl;
            aux = splitStatement(line);
            cout << aux << endl;

            ast_obj->statementClass = static_cast<void*>(assign);
			ast_obj->className = "ASSIGNMENT";

			statement->statement.push_back(ast_obj);

            if(aux.size()+1 < line.size())
            	line = line.substr(aux.size()+1, line.size());
            else
            	break;
            continue;
        }
        break;
    }

    return statement;
}


Program *Arquivo::readFile(vector<string> file_input){
    int n = file_input.size();
    int i;
    int j = 0;
    int z = 0;
    int lineSize;
    vector<string> token;
    string aux;
    string line;

    Program *prog = new Program();

    Function *function;
    FunctionParam *functionParam;
    Variable *var;
    Statement *statement;
    ASTObject *ast_obj;
    Constant *const_;
    IF *if_;
    For *for_;
    While *while_;
    DoWhile *dowhile_;
    Printf *printf_;
    Scanf *scanf_;
    Exit *exit_;
    Return *return_;
    Assignment *assign;



    for(i = 0; i < n; i++){
        lineSize = file_input[i].size();
        line = file_input[i];
        z = 0;
        for(j = 0; j < lineSize; j++){
            if(line[j] == ' '){
                z++;
            }
            if(line[j] == ':'){
                aux = line.substr(z, j-z);
                z = j+1;
                if(aux.compare("FUNCTION") == 0){
                    function = new Function();
                    statement = new Statement();

                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Function name: " << token[1] << endl;

                    function->name = token[1];
                    function->statementList = statement;
                }
                if(aux.compare("RETURN_TYPE") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Return Type: " << strtoupper(token[1]) << endl;

                    function->return_type = token[1];
                }
                if(aux.compare("PARAMETER") == 0){
                    functionParam = new FunctionParam();
                    statement = new Statement();

                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Parameter Name: " << token[1] << endl;
                    cout << "Parameter Type: " << strtoupper(token[3]) << endl;

                    functionParam->name = token[1];
                    functionParam->parm_type = strtoupper(token[3]);

                    function->paramsList.push_back(functionParam);
                    function->statementList = statement;

                }
                if(aux.compare("VARIABLE") == 0){
                	var = new Variable();
                	ast_obj = new ASTObject();

                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Variable Name: " << token[1] << endl;
                    cout << "Variable Type: " << strtoupper(token[3]) << endl;

                    var->name = token[1];
                    var->var_type = strtoupper(token[3]);

                    ast_obj->statementClass = static_cast<void*>(var);
                    ast_obj->className = "VARIABLE";

                    function->statementList->statement.push_back(ast_obj);

                }
                if(aux.compare("GLOBAL VARIABLE") == 0){
                	var = new Variable();
                	ast_obj = new ASTObject();

                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Global Variable Name: " << token[1] << endl;
                    cout << "Global Variable Type: " << strtoupper(token[3]) << endl;

                    var->name = token[1];
                    var->var_type = strtoupper(token[3]);
                    var->category = "GLOBAL_VARIABLE";

                    ast_obj->statementClass = static_cast<void*>(var);
                    ast_obj->className = "VARIABLE";

                    prog->globals.push_back(ast_obj);

                }
                if(aux.compare("CONSTANT") == 0){
                	const_ = new Constant();

                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Constant Name: " << token[1] << endl;
                    cout << "Constant Value: " << token[3] << endl;

                    const_->name = token[1];
                    const_->value = token[3];

                    ast_obj->statementClass = static_cast<void*>(const_);
                    ast_obj->className = "CONSTANT";

                    prog->globals.push_back(ast_obj);
                }
                aux.clear();
                cout << endl;
            }
            if(line[j] == '('){
                aux = line.substr(z, j-z);
                z = j;
                if(aux.compare("IF") == 0){
                    cout << "COMANDO IF: " << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    ifParser(aux);

                    ast_obj->statementClass = static_cast<void*>(if_);
                    ast_obj->className = "IF";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("PRINTF") == 0){
                    cout << "COMANDO PRINTF: " << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    printParser(aux);

                    ast_obj->statementClass = static_cast<void*>(printf_);
                    ast_obj->className = "PRINTF";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("SCANF") == 0){
                    cout << "COMANDO SCANF:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    scanfParser(aux);

                    ast_obj->statementClass = static_cast<void*>(scanf_);
                    ast_obj->className = "SCANF";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("RETURN") == 0){
                    cout << "COMANDO RETURN:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    returnParser(aux);

                    ast_obj->statementClass = static_cast<void*>(return_);
                    ast_obj->className = "RETURN";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("WHILE") == 0){
                    cout << "COMANDO WHILE:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    whileParser(aux);

                    ast_obj->statementClass = static_cast<void*>(while_);
                    ast_obj->className = "WHILE";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("FOR") == 0){
                    cout << "COMANDO FOR:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    forParser(aux);

                    ast_obj->statementClass = static_cast<void*>(for_);
                    ast_obj->className = "FOR";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("EXIT") == 0){
                    cout << "COMANDO EXIT:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    exitParser(aux);

                    ast_obj->statementClass = static_cast<void*>(exit_);
                    ast_obj->className = "EXIT";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("DO_WHILE") == 0){
                    cout << "COMANDO DO_WHILE:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    dowhileParser(aux);

                    ast_obj->statementClass = static_cast<void*>(dowhile_);
                    ast_obj->className = "DOWHILE";

                    function->statementList->statement.push_back(ast_obj);
                }
                if(aux.compare("ts") == 0){
                    cout << "expression:" << endl;


                }
            }
        } 
    }

    return prog;
} 

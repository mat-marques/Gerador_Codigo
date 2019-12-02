#include "Arquivo.h"

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

void printParser(string line){
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
    
}

void scanfParser(string line){
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
}

string verifyExpression(string line, int s){
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
    return ("Pronto");
}

void ifParser(string line){
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
}





void returnParser(string line){
    int n = line.size();
    int i;
    cout << line << endl;
    for(i = 0; i < n; i++){
       

    }


}

void Arquivo::readFile(vector<string> file_input){
    int n = file_input.size();
    int i;
    int j = 0;
    int z = 0;
    int lineSize;
    vector<string> token;
    string aux;
    string line;
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
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Function name: " << token[1] << endl;

                }
                if(aux.compare("RETURN_TYPE") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Return Type: " << strtoupper(token[1]) << endl;
                }
                if(aux.compare("PARAMETER") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Parameter Name: " << token[1] << endl;
                    cout << "Parameter Type: " << strtoupper(token[3]) << endl;
                }
                if(aux.compare("VARIABLE") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Variable Name: " << token[1] << endl;
                    cout << "Variable Type: " << strtoupper(token[3]) << endl;
                
                }
                if(aux.compare("LOBAL VARIABLE") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Global Variable Name: " << token[1] << endl;
                    cout << "Global Variable Type: " << strtoupper(token[3]) << endl;
                }
                if(aux.compare("CONSTANT") == 0){
                    aux.clear();
                    aux = line.substr(j, line.size());
                    token = splitText(aux, " ");
                    cout << "Constant Name: " << token[1] << endl;
                    cout << "Constant Value: " << strtoupper(token[3]) << endl;
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
                
                }
                if(aux.compare("PRINTF") == 0){
                    cout << "COMANDO PRINTF: " << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    printParser(aux);
                
                }
                if(aux.compare("SCANF") == 0){
                    cout << "COMANDO SCANF:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    scanfParser(aux);
                
                }
                if(aux.compare("RETURN") == 0){
                    cout << "COMANDO RETURN:" << endl;
                    aux.clear();
                    aux = line.substr(z, line.size());
                    returnParser(aux);
                }
                if(aux.compare("WHILE") == 0){
                    cout << "COMANDO WHILE:" << endl;
                
                
                }
                if(aux.compare("FOR") == 0){
                    cout << "COMANDO FOR:" << endl;
                
                
                }
                if(aux.compare("EXIT") == 0){
                    cout << "COMANDO EXIT:" << endl;
                
                
                }
                if(aux.compare("DO_WHILE") == 0){
                    cout << "COMANDO DO_WHILE:" << endl;
                
                
                }
                if(aux.compare("ts") == 0){
                    cout << "expression:" << endl;
                    string teste = verifyExpression(line, 0);
                    teste = verifyExpression(teste, 0);

                }
            }
        } 
    }
} 
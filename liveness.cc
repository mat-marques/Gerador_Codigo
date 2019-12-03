#include "liveness.h"
#include <string>
#include <sstream>
#include <algorithm>


void showlist(list <string> g) 
{ 
    list <string> :: iterator it; 
    for(it = g.begin(); it != g.end(); ++it)
            cout << "    " << *it;  
} 

class defUse
{
    public:
        string label;
        string operacao;
        list<string> definicoes;
        list<string> usando;
        list<string> in,out;

        defUse(){};
        ~defUse(){};

        void addUse(string use)
        {
            usando.push_back(use);
        }

        void addDef(string def)
        {
            definicoes.push_back(def);
        }
};

class Block
{
    public:
        string Label;
        list<defUse*> defAndUse;
        list<string> in,out;
        bool onFunction;
        void addDefUse(defUse *deUse)
        {
            defAndUse.push_back(deUse);
        }
        Block(){};
        ~Block(){};
};

void livenessVariableProcess(graph* graficoInteiros, graph* graficoInteiroTemporario, Program *p)
{
     list<Block*> fimBloco;
     std::list<Block*> blocos;
     std::list<Block*> blocosFunc;
     std::list<Block*> fimBlocoFunc;
     std::list<Block*> fimBlocoMain;
     list<defUse*> defUses;
     std::list<MipsInstruction*>:: iterator node;
     std::list<defUse*>:: iterator node2;
     std::list<Block*>:: iterator nodeBloco;
     std::list<string>:: iterator def,use;
     std::list<Function*>:: iterator nodeFunc;
     Block *comeco;
     int change = 1;
     int newblock = 1;
     string actualLabel = "macacoDaUel";

        cout << "tamanho da lista:" << instructionList.size() << "\n";
        for(node = instructionList.begin(); node != instructionList.end(); node++)
        {
            if(newblock || actualLabel != (*node)->label)
            {
                comeco = new Block();
                comeco->Label = (*node)->label;
                cout << "Label: " << (*node)->label << "\n";
                for(nodeFunc = p->functionsList.begin(); nodeFunc != p->functionsList.end(); nodeFunc++)
                {   
                    string auxString;
                    auxString = (*node)->label;
                    //cout << "Entrou Here: " << (*nodeFunc)->name << "\n";
                    if(auxString.find((*nodeFunc)->name) != std::string::npos && (*nodeFunc)->name != "main")
                    {
                        //cout << "Entrou Here2: " << (*nodeFunc)->name << "\n";
                        cout << (*node)->instruction << " " << (*node)->register1 << " " << (*node)->register2 << " " << (*node)->register3 << "\n";
                        blocosFunc.push_back(comeco);
                        newblock = 0;
                        break;
                    }
                }
                if(newblock)
                {
                    blocos.push_back(comeco);
                    newblock = 0;
                    //cout << "Entrou aq\n";
                    cout << (*node)->instruction << " " << (*node)->register1 << " " << (*node)->register2 << " " << (*node)->register3 << "\n";
                }
                if(actualLabel != (*node)->label)
                actualLabel = (*node)->label;
            }

            defUse *definicoes = new defUse();

            if((*node)->instruction == "add" || (*node)->instruction == "sub" || (*node)->instruction == "mul" || (*node)->instruction == "nor")
            {   
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                if((*node)->register1 == (*node)->register2 || (*node)->register1 == (*node)->register3)
                {
                    definicoes->addUse((*node)->register2);
                    definicoes->addUse((*node)->register3);    
                }
                else if((*node)->register2 == (*node)->register3)
                {
                    definicoes->addDef((*node)->register1);
                    definicoes->addUse((*node)->register2);
                }
                else
                {
                    definicoes->addDef((*node)->register1);
                    definicoes->addUse((*node)->register2);
                    definicoes->addUse((*node)->register3);   
                }
            }
            else if((*node)->instruction == "addi" || (*node)->instruction == "subi" || (*node)->instruction == "move")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                std::istringstream arg ((*node)->register2);
                int verification;
                arg >> verification;
                if((*node)->register1 == (*node)->register2)
                {
                    definicoes->addUse((*node)->register2);
                }
                else if((*node)->register1 == (*node)->register3)
                {
                    definicoes->addUse((*node)->register3);
                }
                else if(arg.fail())
                {
                    definicoes->addDef((*node)->register1);
                    definicoes->addUse((*node)->register2);
                }
                else
                {
                    definicoes->addDef((*node)->register1);
                    definicoes->addUse((*node)->register3);                    
                }
            }
            else if((*node)->instruction == "mult" || (*node)->instruction == "div")
            {   
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                if((*node)->register1 == (*node)->register2)
                {
                    definicoes->addUse((*node)->register2);
                }
                else
                {
                    definicoes->addUse((*node)->register1);
                    definicoes->addUse((*node)->register2);
                }
            }
            else if((*node)->instruction == "lw")
            {   
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                definicoes->addDef((*node)->register1);
            }
            else if((*node)->instruction == "la" ||  (*node)->instruction == "li" ||  (*node)->instruction == "mfhi" || (*node)->instruction == "mflo")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                definicoes->addDef((*node)->register1);
                if((*node)->instruction == "li" && (*node)->register2 == "10")
                {
                    Block *fim = new Block();
                    fim->defAndUse.push_back(definicoes);
                    fim->Label = (*node)->label;
                    fimBlocoMain.push_back(fim);
                    comeco->onFunction = true;
                }
            }
            else if((*node)->instruction == "sw")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                if((*node)->instruction != "$ra")
                {
                    definicoes->addUse((*node)->register1);
                }
            }
            else if((*node)->instruction == "beq" || (*node)->instruction == "bne" || (*node)->instruction == "bgt" || (*node)->instruction == "bge" || (*node)->instruction == "blt" || (*node)->instruction == "ble" || (*node)->instruction == "bnq")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
               // Block *fim = new Block();
                if((*node)->register1 == (*node)->register2)
                {
                    definicoes->addUse((*node)->register1);
                }
                else
                {
                    definicoes->addUse((*node)->register1);
                    definicoes->addUse((*node)->register2);
                }
                //fim->Bloco = (*node);
               //fim->Label = (*node)->label;
               // fimBloco.push_back(fim);
                newblock = 1;
            }
            else if((*node)->instruction == "j")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                //definicoes->jump = (*node)->register1;
               // Block *fim = new Block();
                //fim->Bloco = (*node);
               // fim->Label = (*node)->label;
               // fimBloco.push_back(fim);
                newblock = 1;
            }
            else if((*node)->instruction == "jr")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                Block *fim = new Block();
                fim->defAndUse.push_back(definicoes);
                fim->Label = (*node)->label;
                fimBlocoFunc.push_back(fim);
                comeco->onFunction = true;
                newblock = 1;
                
            }
            else if((*node)->instruction == "jal")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
                newblock = 1;
                for(nodeBloco = blocos.begin(); nodeBloco != blocos.end(); nodeBloco++)
                {
                    if((*nodeBloco)->Label == (*node)->register1)
                    {
                        (*nodeBloco)->onFunction = true;
                    }
                }
            }
            else if((*node)->instruction == "syscall")
            {
                definicoes->operacao = (*node)->instruction;
                definicoes->label = (*node)->label;
            }
            else
            {
                std::cout << "NAO ENCONTRADO INSTRUCAO: " << (*node)->instruction << "\n";
            }
            comeco->addDefUse(definicoes);
        }
        int iteracao = 0;
        blocos.reverse();
        while(change)
        {   
            change = 0;
            string actualLabel;
            list<string> actualInOut;
            actualLabel = "macacoDaUel";
            for(nodeBloco = blocos.begin(); nodeBloco != blocos.end(); nodeBloco++)
            {
                std::list<Block*>::iterator nodeFimBlocoMain;
                for(nodeFimBlocoMain = fimBlocoMain.begin(); nodeFimBlocoMain != fimBlocoMain.end(); nodeFimBlocoMain++)
                {
                    if((*nodeBloco)->Label == (*nodeFimBlocoMain)->Label)
                    for(node2 = (*nodeBloco)->defAndUse.begin(); node2 != (*nodeBloco)->defAndUse.end(); node2++)
                    {
                        /*cout << "label: " << (*node2)->label;
                        cout << "\ninstruction: " << (*node2)->operacao;
                        cout << "\ndef: ";
                        showlist((*node2)->definicoes);
                        cout << "\nuse: ";
                        showlist((*node2)->usando);
                        cout << "\n";*/
                        
                    }
                }

                //cout << "\n\nSAIU!!\n\n";
                /*if(actualLabel != (*node2)->label)
                {
                    if(actualLabel != "macacoDaUel")
                    {
                        for( nodeBloco = blocos.begin(); nodeBloco != blocos.end(); nodeBloco++)
                        {
                            if((*nodeBloco)->Label == actualLabel)
                            {
                                if((*nodeBloco)->in != actualInOut)
                                {
                                    (*nodeBloco)->in = actualInOut;
                                    change = 1;
                                }
                                break;
                            }
                        }
                    }
                    actualLabel = (*node2)->label;
                    if((*node2)->operacao == "j" || (*node2)->operacao == "beq" || (*node2)->operacao == "bne" || (*node2)->operacao == "bgt" || (*node2)->operacao == "bge" || (*node2)->operacao == "blt" || (*node2)->operacao == "ble" || (*node2)->operacao == "bnq")
                    {
                        for(nodeBloco = blocos.begin(); nodeBloco != blocos.end(); nodeBloco++ )
                        {
                            //if((*nodeBloco)->label == )
                        }
                    }
                }
                if(node2 == defUses.begin() && iteracao == 0)
                {
                    (*node2)->in.merge((*node2)->usando);
                    (*node2)->in.unique();
                }
                else
                {
                    if((*node2)->operacao != "j" && (*node2)->operacao != "jal" && (*node2)->operacao != "jr" && (*node2)->operacao != "beq" && (*node2)->operacao != "bne" && (*node2)->operacao != "bgt" && (*node2)->operacao != "bge" && (*node2)->operacao != "blt" && (*node2)->operacao != "ble" && (*node2)->operacao != "bnq")
                    {

                    }
                }*/
            }
            iteracao++;
        }
}
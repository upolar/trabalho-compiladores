#include <fstream>
#include <vector>
#include "symboltable.h"

using namespace std;

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
        SymbolTable* st;
        vector<Token*> lookaheadBuffer;
    public:
        Token* lookAhead(int n);
        Token* consumeToken();
    //Construtor
        Scanner(string, SymbolTable*);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};

#include "scanner.h"
#include <cctype>
#include <cstdlib>    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable* table)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;
    st = table;
    
    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;
    string current;
    int input_length = static_cast<int>(input.length());

    while(pos < input_length && isspace(input[pos])) {
        if (input[pos] == '\n')
            line++;
        pos++;

    }
        
    
    if (pos >= input_length) 
    { // EOF
        tok = new Token(END_OF_FILE);
    } 
    else if(isalpha(input[pos]) || input[pos] == '_')
    { // ID
        lexeme.push_back(input[pos]);
        pos++;
        while(isalnum(input[pos]) || input[pos] == '_')
        {
            lexeme.push_back(input[pos]);
            pos++;
        }

        STEntry* obj = st->get(lexeme);
        if (!obj)
            tok = new Token(ID, lexeme);
        else 
            tok = new Token(obj->token->name);
    } 
    else if (isdigit(input[pos])) 
    { // INTEGER_LITERAL
    
        while(isdigit(input[pos]))
        {
            lexeme.push_back(input[pos]);
            pos++;
        }
            
        tok = new Token(INTEGER_LITERAL, lexeme);
    } 
    else if (input[pos] == '<') 
    {
        if (pos + 1 < input_length && input[pos+1] == '=') {
            tok = new Token(LE);
            pos +=2;
        } else {
            tok = new Token(LT);
            pos++;
        }
        
    }
    else if (input[pos] == '>') 
    {
        if (pos + 1 < input_length && input[pos+1] == '=') {
            tok = new Token(GE);
            pos +=2;
        } else {
            tok = new Token(GT);
            pos++;
        }
    }
    else if (input[pos] == '+')
    {
        tok = new Token(PLUS);
        pos++;
    } 
    else if (input[pos] == '-') 
    {
        tok = new Token(MINUS);
        pos++;
    }
    else if (input[pos] == '*') 
    {
        tok = new Token(MULT);
        pos++;
    }
    else if (input[pos] == '/') 
    { 
        if (pos + 1 < input_length && input[pos + 1] == '/') 
        { 
            while(pos < input_length && input[pos] != '\n')
                pos++;
                
            if (pos < input_length && input[pos] == '\n')
            {
                pos++;
                line++;
            }
            return nextToken();
        } 
        else if (pos + 1 < input_length && input[pos + 1] == '*') 
        { 
            pos += 2; 
            bool found = false;
            while(pos + 1 < input_length)
            {
                if (pos < input_length && input[pos] == '\n') 
                {
                    pos++;
                    line++;
                }

                if (input[pos] == '*' && input[pos + 1] == '/')
                {
                    pos += 2;
                    found = true;
                    return nextToken();
                    break;
                    
                } 
                pos++;
            }
                
            

            if (!found)
            {
                lexicalError("Comentário de bloco não fechado: esperado '*/'");
            }

            
        } else {
            tok = new Token(DIV);
            pos++;
        }
    } 
    else if (input[pos] == '%') 
    {
        tok = new Token(MOD);
        pos++;
    }
    else if (input[pos] == '=')
    {
        if (pos + 1 < input_length && input[pos+1] == '=') {
            tok = new Token(EQ);
            pos+=2;
        } else {
            tok = new Token(ASSIGN);
            pos++;
        }
    } 
    else if (input[pos] == '!')
    {
        if (pos + 1 < input_length && input[pos+1] == '=') {
            tok = new Token(NEQ);
            pos+=2;
        } else {
            lexicalError("Token não reconhecido!");
        }
    }
    else if (input[pos] == '(') 
    {
        tok = new Token(LPAREN);
        pos++;
    }
    else if (input[pos] == ')') 
    {
        tok = new Token(RPAREN);
        pos++;
    }
    else if (input[pos] == '[') 
    {
        tok = new Token(LBRACKET);
        pos++;
    }
    else if (input[pos] == ']') 
    {
        tok = new Token(RBRACKET);
        pos++;
    }
    else if (input[pos] == '{') 
    {
        tok = new Token(LBRACE);
        pos++;
    }
    else if (input[pos] == '}') 
    {
        tok = new Token(RBRACE);
        pos++;
    }
    else if (input[pos] == ';')
    {
        tok = new Token(SEMICOLON);
        pos++;
    }
    else if (input[pos] == '.')
    {
        tok = new Token(DOT);
        pos++;
    }
    else if (input[pos] == ',')
    {
        tok = new Token(COMMA);
        pos++;
    }
    else if (input[pos] == '"') 
    {
        lexeme.push_back(input[pos]);
        pos++;

        while (pos < input_length && input[pos] != '"')
        {
            lexeme.push_back(input[pos]);
            pos++;

            if (pos < input_length && input[pos] == '\n')
            {
                pos++;
                line++;
            }
        }

        if (pos < input_length) 
        {
        
            if (input[pos] == '"')
            {
                lexeme.push_back(input[pos]);
                tok = new Token(STRING_LITERAL, lexeme);
                pos++;
            } 
            
        } 
        else {
            lexicalError("Esperava um \" de fechamento de string.");
        }

    } else {
        lexicalError("Token não reconhecido: " + string(1, input[pos]));
    }

    //cout << tok->lexeme;

    return tok;
 
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}

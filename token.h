#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    ID,
    INTEGER_LITERAL,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    ASSIGN,
    EQ,
    NEQ,
    RPAREN,
    LPAREN,
    RBRACE,
    LBRACE,
    RBRACKET,
    LBRACKET,
    SEMICOLON,
    COMMA,
    DOT,
    STRING_LITERAL,
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};

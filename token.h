#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    ID,
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
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    LBRACKET, // [
    RBRACKET, // ]
    SEMICOLON, 
    COMMA, 
    DOT, 
    INTEGER_LITERAL,
    STRING_LITERAL,
    END_OF_FILE,
    CLASS, EXTENDS, INT, STRING, BREAK, PRINT, READ, RETURN, SUPER, IF, ELSE, FOR, NEW, CONSTRUCTOR
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

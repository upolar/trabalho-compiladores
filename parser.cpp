#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
	//TODO
	if (lToken.name == CLASS)
    	classList();
}

//Continuar....

void
Parser::classList()
{
    do { 
		ClassDecl();
	} while (lToken->name == CLASS);
}

void
Parser::ClassDecl()
{
	match(CLASS);
	match(ID);
	if (lToken->name == EXTENDS) 
	{
		match(EXTENDS);
		match(ID);
	}
	classBody();
}

void
Parser::classBody()
{
	match(LBRACE);
	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();
	match(RBRACE);
}

void
Parser::varDeclListOpt()
{
	if (lToken->name == TYPE) {
		varDeclList();
	}

}

Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}

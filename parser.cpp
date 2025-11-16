#include "parser.h"

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSimbolTable();

	scanner = new Scanner(input, globalST);
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
	{
		error("Erro inesperado");
	}
		
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::initSimbolTable()
{
	Token* t;
        
	t = new Token(CLASS, "class");
	globalST->add(new STEntry(t, true));
	t = new Token(EXTENDS, "extends");
    globalST->add(new STEntry(t, true));
	t = new Token(INT, "int");
    globalST->add(new STEntry(t, true));
	t = new Token(STRING, "string");
    globalST->add(new STEntry(t, true));
	t = new Token(BREAK, "break");
	globalST->add(new STEntry(t, true));
	t = new Token(PRINT, "print"); 
	globalST->add(new STEntry(t, true));
	t = new Token(READ, "read"); 
	globalST->add(new STEntry(t, true));
	t = new Token(RETURN, "return");
	globalST->add(new STEntry(t, true));
	t = new Token(SUPER, "super");
	globalST->add(new STEntry(t, true));
	t = new Token(IF, "if");
	globalST->add(new STEntry(t, true));
	t = new Token(ELSE, "else");
	globalST->add(new STEntry(t, true));
	t = new Token(FOR, "for");
	globalST->add(new STEntry(t, true));
	t = new Token(NEW, "new");
	globalST->add(new STEntry(t, true));
	t = new Token(CONSTRUCTOR, "constructor");
	globalST->add(new STEntry(t, true));
}
 
// Funções auxiliares
bool
Parser::isType()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		return true;
	}
	return false;
}	

void
Parser::brackets()
{
	if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		match(RBRACKET);
	}
}

bool
Parser::isExpression()
{
	int allowedExpression[] = {
		PLUS,
		MINUS
	};

	for (unsigned int i = 0; i < sizeof(allowedExpression)/sizeof(int); i++)
		if (lToken->name == allowedExpression[i])
			return true;
	return false;
}

bool
Parser::isStat()
{
	int stats[] = {
		PRINT, // Produção 24
		READ, // Produção 25
		RETURN, // Produção 26
		SUPER, // Produção 27
		IF, // Produção 28
		FOR,  // Produção 29
		ID, // Produção 23 e 32
		SEMICOLON
	};

	for (long unsigned int i = 0; i < sizeof(stats)/sizeof(int); i++) 
	{
		if (lToken->name == stats[i])
			return true;
	}
	return false;
	
}

bool
Parser::isRelOp()
{
	int allowedRelOps[] = {
		LT, // <
    	GT, // >
    	LE, // <=
    	GE, // >=
		EQ, // ==
		NEQ // !=
	};

	for (long unsigned int i = 0; i < sizeof(allowedRelOps)/sizeof(int); i++)
		if (lToken->name == allowedRelOps[i])
			return true;
	return false;
}

// Funções principais	
void
Parser::program()
{
	if (lToken->name == CLASS)
	{
    	classList();
	}
}

void
Parser::classList()
{
    do { 
		classDecl();
	} while (lToken->name == CLASS);
}

void
Parser::classDecl()
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
	if (isType()) {
		varDeclList();
	}

}

void
Parser::varDeclList()
{
	varDecl();
	varDeclListLinha();
}

void
Parser::varDeclListLinha()
{
	if(isType())
	{
		varDecl();
		varDeclListLinha();
	}
}

void
Parser::varDecl()
{
	type();
	brackets();
	match(ID);
	varDeclOpt();
	match(SEMICOLON);
}

void
Parser::varDeclOpt()
{
	if (lToken->name == COMMA)
	{
		match(COMMA);
		match(ID);
		varDeclOpt();
	}
}

void
Parser::type()
{
	if (isType()) 
		match(lToken->name);
}
void
Parser::constructDeclListOpt()
{
	if (lToken->name == CONSTRUCTOR)
		constructDeclList();
}
 
void 
Parser::constructDeclList()
{
	constructDecl();
	constructDeclListLinha();
}

void 
Parser::constructDeclListLinha()
{
	if (lToken->name == CONSTRUCTOR)
	{
		constructDecl();
		constructDeclListLinha();
	}
}

void 
Parser::constructDecl()
{
	match(CONSTRUCTOR);
	methodBody();
}

void
Parser::methodBody()
{
	match(LPAREN);
	paramListOpt();
	match(RPAREN);
	match(LBRACE);
	statementsOpt();
	match(RBRACE);
}

void
Parser::methodDeclListOpt()
{
	if (isType()) {
		methodDeclList();
	}
}

void
Parser::methodDeclList()
{
	methodDecl();
	methodDeclListLinha();
}

void
Parser::methodDeclListLinha()
{
	if (isType()) {
		methodDecl();
		methodDeclListLinha();
	}
}

void
Parser::methodDecl()
{
	type();
	brackets();
	match(ID);
	methodBody();
}

void
Parser::paramListOpt()
{
	if (isType())
		paramList();
}

void
Parser::paramList()
{
	param();
	paramListLinha();
}

void
Parser::paramListLinha()
{
	if (lToken->name == COMMA)
	{
		param();
		paramListLinha();
	}
}

void
Parser::param()
{
	type();
	brackets();
	match(ID);
}

void
Parser::statementsOpt()
{
	if (isStat())
		statements();
}

void
Parser::statements() {
	statement();
	statementsLinha();
}

void
Parser::statementsLinha()
{
	if(isStat()) 
	{
		statement();
		statementsLinha();
	}
}

void
Parser::statement() 
{
	if (isType()) 
		varDeclList();
	else if (lToken->name == ID) 
		atribStat();
	else if (lToken->name == PRINT)
	{
		match(PRINT);
		expression();
	}
	else if (lToken->name == READ)
	{
		match(READ);
		lValue();
	}
	else if (lToken->name == RETURN)
	{
		match(RETURN);
		expression();
	}
	else if (lToken->name == SUPER)
	{
		match(SUPER);
		match(LPAREN);
		argListOpt();
		match(RPAREN);
	}
	else if (lToken->name == IF)
		ifStat();
	else if (lToken->name == FOR)
		forStat();
	else if (lToken->name == BREAK)
	{
		match(BREAK);
		match(SEMICOLON);
	}
	else if (lToken->name == SEMICOLON)
	{
		match(SEMICOLON);
	}
}

void
Parser::atribStat()
{
	lValue();
	match(ASSIGN);
	if (lToken->name == NEW || isType()) {
		allocExpression();
	} else {
		expression();
	}
}

void
Parser::ifStat()
{
	match(IF);
	match(LPAREN);
	expression();
	match(RPAREN);
	match(LBRACE);
	statements();
	match(RBRACE);
	if (lToken->name == ELSE) {
		match(ELSE);
		match(LBRACE);
		statements();
		match(RBRACE);
	}
}

void
Parser::forStat() 
{
	match(FOR);
	match(LPAREN);
	atribStatOpt();
	match(SEMICOLON);
	expressionOpt();
	match(SEMICOLON);
	atribStatOpt();
	match(RPAREN);
	match(LBRACE);
	statements();
	match(RBRACE);
}

void
Parser::atribStatOpt()
{
	if (lToken->name == ID)
		atribStat();
}

void
Parser::expressionOpt()
{
	if (isExpression()) 
	{
		expression();
	}
}

void
Parser::lValue() 
{
	match(ID);
	lValueComp();
}

void
Parser::lValueComp() 
{
	if (lToken->name == DOT)
	{
		match(DOT);
		match(ID);
		if (lToken->name == LBRACKET)
		{
			expression();
			match(RBRACKET);
		}
		else if (lToken->name == LPAREN)
		{
			argListOpt();
			match(RPAREN);
		}
		lValueComp();
	} 
	else if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		expression();
		match(RBRACKET);
		lValueComp();
	}
}

void
Parser::expression()
{
	numExpression();
	if (isRelOp())
	{
		match(lToken->name);
		numExpression();
	}
}

void
Parser::allocExpression()
{
	if (lToken->name == NEW) 
	{
		match(NEW);
		match(ID);
		match(LPAREN);
		argListOpt();
		match(RPAREN);
	} 
	else if (isType()) 
	{
		match(lToken->name);
		match(LBRACKET);
		expression();
		match(RBRACKET);
	} 
}

void
Parser::numExpression()
{
	term();
	if (lToken->name == PLUS || lToken->name == MINUS)
	{
		match(lToken->name);
		term();
	}
	
}

void
Parser::term()
{
	unaryExpression();
	if (lToken->name == MULT || lToken->name == DIV || lToken->name == MOD)
	{
		match(lToken->name);
		unaryExpression();
	}
}

void
Parser::unaryExpression()
{
	if (lToken->name == PLUS) 
	{
		match(PLUS);
	} 
	else {
		match(MINUS);
	}
	factor();
}

void
Parser::factor()
{
	if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL)
		match(lToken->name);
	else if (lToken->name == ID)
		lValue();
	else if (lToken->name == LPAREN) 
	{
		match(LPAREN);
		expression();
		match(RPAREN);
	}
}

void
Parser::argListOpt()
{
	if (isExpression()) {
		argList();
	}
		
}

void
Parser::argList()
{
	expression();
	argListLinha();
}

void
Parser::argListLinha()
{
	if (lToken->name == COMMA)
	{
		match(COMMA);
		expression();
		argListLinha();
	}
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}

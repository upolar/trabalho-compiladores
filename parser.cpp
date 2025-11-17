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
	lToken = scanner->consumeToken();
}

string
Parser::tokenName(int t)
{
	switch (t) {
		case UNDEF: 
			return "UNDEF";

		case ID: 
			return "ID";
		
		case INTEGER_LITERAL: 
			return "INTEGER_LITERAL";
		case STRING_LITERAL: 
			return "STRING_LITERAL";
		case CLASS: 
			return "class";
		case EXTENDS: 
			return "extends";
		case INT: 
			return "int";
		case STRING: 
			return "string";
		case BREAK: 
			return "break";
		case PRINT: 
			return "print";
		case READ: 
			return "read";
		case RETURN: 
			return "return";
		case SUPER: 
			return "super";
		case IF: 
			return "if";
		case ELSE: 
			return "else";
		case FOR: 
			return "for";
		case NEW: 
			return "new";
		case CONSTRUCTOR: 
			return "constructor";

		case LT: 
			return "<";
		case GT: 
			return ">";
		case LE: 
			return "<=";
		case GE: 
			return ">=";
		case PLUS: 
			return "+";	
		case MINUS: 
			return "-";
		case MULT: 
			return "*";
		case DIV: 
			return "/";
		case MOD: 
			return "%";
		case ASSIGN: 
			return "=";
		case EQ: 
			return "==";
		case NEQ: 
			return "!=";
		case LPAREN: 
			return "(";
		case RPAREN: 
			return ")";
		case LBRACE: 
			return "{";
		case RBRACE: 
			return "}";
		case LBRACKET: 
			return "[";
		case RBRACKET: 
			return "]";
		case SEMICOLON: 
			return ";";
		case COMMA: 
			return ",";
		case DOT: 
			return ".";
		case END_OF_FILE: 
			return "END_OF_FILE";
		
		default: 
			return "";
	}
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
	{
		if (tokenName(t) != "") {
			error("Erro inesperado: esperava receber o token " + tokenName(t) + " e recebi o token " + tokenName(lToken->name));
		} else {
			error("Erro inesperado");
		}
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
	return (lToken->name == INTEGER_LITERAL ||
		lToken->name == STRING_LITERAL ||
		lToken->name == ID ||
		lToken->name == LPAREN ||
		lToken->name == PLUS ||
		lToken->name == MINUS 
	);
}

bool
Parser::isStat()
{
	return (
		isType() ||
		lToken->name == ASSIGN ||
		lToken->name == PRINT ||
		lToken->name == READ ||
		lToken->name == RETURN ||
		lToken->name == SUPER ||
		lToken->name == IF ||
		lToken->name == FOR ||
		lToken->name == BREAK ||
		lToken->name == SEMICOLON 
	);
	
}

bool
Parser::isRelOp()
{
	int allowedRelOps[] = {
		LT,
    	GT,
    	LE, 
		GE, 
		EQ, 
		NEQ 
	};

	for (long unsigned int i = 0; i < sizeof(allowedRelOps)/sizeof(int); i++)
		if (lToken->name == allowedRelOps[i])
			return true;
	return false;
}

bool
Parser::isVarDecl()
{
	if (!isType()) {
		return false;
	}
	
	int lookaheadPos = 1; 
	Token* lookaheadToken;
	
	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (lookaheadToken && lookaheadToken->name == LBRACKET) {
		lookaheadPos++; 
		lookaheadToken = scanner->lookAhead(lookaheadPos);
		if (lookaheadToken && lookaheadToken->name == RBRACKET) {
			lookaheadPos++;
		} else {
			return false;
		}
	}
	
	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (!lookaheadToken || lookaheadToken->name != ID) {
		return false;
	}
	lookaheadPos++;
	
	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (!lookaheadToken) {
		return false;
	}
	
	if (lookaheadToken->name == LPAREN) {
		return false;
	}
	
	if (lookaheadToken->name == COMMA || lookaheadToken->name == SEMICOLON) {
		return true;
	}
	
	return false;
}

bool
Parser::isMethodDecl()
{

	if (!isType()) {
		return false;
	}
	
	int lookaheadPos = 1;
	Token* lookaheadToken;

	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (lookaheadToken && lookaheadToken->name == LBRACKET) {
		lookaheadPos++;
		lookaheadToken = scanner->lookAhead(lookaheadPos);
		if (lookaheadToken && lookaheadToken->name == RBRACKET) {
			lookaheadPos++;
		} else {
			return false; 
		}
	}

	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (!lookaheadToken || lookaheadToken->name != ID) {
		return false;
	}
	lookaheadPos++;

	lookaheadToken = scanner->lookAhead(lookaheadPos);
	if (!lookaheadToken) {
		return false;
	}

	if (lookaheadToken->name == LPAREN) {
		return true;
	}
	
	return false;
}

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

	if (isVarDecl()) {
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

	if(isVarDecl())
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
	if (lToken->name == SEMICOLON) {
		match(SEMICOLON);
	}
		
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

	if (isMethodDecl()) 
	{
		methodDeclList();
	}
	else if (lToken->name == LPAREN)
	{
		methodBody();
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
	if (isMethodDecl()) {
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
		match(COMMA);
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

	if (lToken->name == ID) 
	{
		atribStat();
		match(SEMICOLON);
	}
	else if (isType()) 
	{
		varDeclList();
	}
	else if (lToken->name == PRINT)
	{
		match(PRINT);
		expression();
		match(SEMICOLON);
	}
	else if (lToken->name == READ)
	{
		match(READ);
		lValue();
		match(SEMICOLON);
	}
	else if (lToken->name == RETURN)
	{
		match(RETURN);
		expression();
		match(SEMICOLON);
	}
	else if (lToken->name == SUPER)
	{
		match(SUPER);
		match(LPAREN);
		argListOpt();
		match(RPAREN);
		match(SEMICOLON);
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
	if (lToken->name == NEW) {
		allocExpression();
	} else if (lToken->name == INT || lToken->name == STRING) {
		allocExpression();
	} else if (lToken->name == ID) {
		Token* lookaheadToken = scanner->lookAhead(1);
		if (lookaheadToken && lookaheadToken->name == LBRACKET) {
			allocExpression();
		} else {
			expression();
		}
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
			match(LBRACKET);
			expression();
			match(RBRACKET);
		}
		else if (lToken->name == LPAREN)
		{
			match(LPAREN);
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
	if (lToken->name == PLUS || lToken->name == MINUS) 
		match(lToken->name);
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
	else
	{
		error("Esperava um factor (INTEGER_LITERAL, STRING_LITERAL, ID ou LPAREN), mas recebi " + tokenName(lToken->name));
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

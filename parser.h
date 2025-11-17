#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
        SymbolTable* globalST;
    	SymbolTable* currentST;

		void advance();
		void match(int);

		bool isType();
        
        // funções auxiliares
        void type();
        void brackets();
        bool isStat();
        bool isRelOp();
        bool isExpression();
        bool isVarDecl(); 
        bool isMethodDecl();

		void initSimbolTable();

        void classList();
        void classDecl();
        void classBody();
        
        void varDeclListOpt();
        void varDeclList();
        void varDeclListLinha();
        void varDecl();
        void varDeclOpt();
        
        
        void constructDeclListOpt();
        void constructDeclList();
        void constructDeclListLinha();
        void constructDecl();
        
        void methodBody();
        void methodDeclListOpt();
        void methodDeclList();
        void methodDeclListLinha();
        void methodDecl();
        
        void paramListOpt();
        void paramList();
        void paramListLinha();
        void param();
        
        void statements();
        void statementsOpt();
        void statementsLinha();
        void statement();

        void atribStat();
        void ifStat();
        void forStat();
        void atribStatOpt();
        void expressionOpt();
        void lValue();
        void lValueComp();    
        void expression();
        void allocExpression();
        void numExpression();
        void term();
        void unaryExpression();
        void factor();
        void argListOpt();
        void argList();
        void argListLinha();

        string tokenName(int);
        void error(string);
        void program();

	public:
		Parser(string);
		void run();


};
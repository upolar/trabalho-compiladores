#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);
		bool isKeyword(string word);
		bool isType();
		
	public:
		Parser(string);
		~Parser();
		void run();
        void program();
        void classList();
        void classDecl();
        void classBody();
        void varDeclListOpt();
        void varDeclList();
        void varDecl();
        void varDeclOpt();
        void type();
        void constructDeclListOpt();
        void methodDeclListOpt();
        void error(string);
};
#include "local.hpp"
#include "lexer.hpp"
#include "parser.hpp"

// Recursive Descent Parsing
// Each nonterminal in the grammer is implemented by a function in the program.
// Each such function looks at the next input symbol in order to choose one of the prductions for the nonterminal.

/* -----------
	Terminal symbols:

	Identifiers such as x, Integer constatns like 12, string constants "foo", special symbols(x and *) etc,
	and keywords like let, in, end...

	Non-terminal symbols A, denoting the abstract syntax classes
	
	Example of grammer:

	Expr -> Expr - Expr
	Expr -> Expr * Expr
	Expr -> Expr + Expr
	Expr -> VAR
	Expr -> INT
	Expr -> -INT
	Expr -> ( Expr )
	Expr -> let VAR = Expr in Expr end 

	A well-formed expression is followed by end-of-file

	Main -> Expr EOF

	Our interpreter grammer:
	@Note: Primitives are binary operations and the likes.
		   The string being the single character operator +, *, etc..

	Expr    Name   : Type
	-----------------------
	Expr -> Int    : int
	Expr -> Bool   : bool
	Expr -> Var    : string
	Expr -> Let    : string * Expr * Expr
	Expr -> Prim   : string * Expr * Expr
	Expr -> If     : Expr * Expr * Expr
	Expr -> Letfun : string * string * Expr * Expr
	Expr -> Call   : Expr * Expr
	-----------------------


   ----------- */

static bool test_next(LexerState* ls, int c) {
	if (ls->t.token == c) {
		next(ls);
		return true;
	}
	else {
		return false;
	}
}

static void check_match(LexerState* ls, int what, int who, int where) {
	if (!test_next(ls, what)) {
		if (where == ls->lineNumber) {
			FatalError("SyntaxError: Failed when checking match %d", what);
		}
		else {
			FatalError("SyntaxError: %c expected (to close to %c at line %d)", (char)what, (char)who, where);
		}
	}
}


static void Expression(LexerState* ls) {
	SubExpression(ls, 0);
}

static void SubExpression(LexerState* ls, int limit) {
	BinOp op;
	SimpleExpression(ls);
}

static void SimpleExpression(LexerState* ls) {
	switch (ls->t.token) {
	case TK_INT:
		{
			
		} break;
	default:
		SuffixedExpression(ls);
		return;
	}
}

static void PrimaryExpression(LexerState* ls)
{
	switch(ls->t.token) {
	case '(': 
		{
			int line = ls->lineNumber;
			next(ls);

			// Parse the expression
			Expression(ls);

			// After parsing the expression inside the funcargs block, we expect a ')'.
			// Check to see if we get the match, if not a FatalError is called.
			check_match(ls, ')', '(', line);

			// @TODO:
			// We should be gathering some args here, and storing them correctly.

			return;
		} break;
	case TK_VAR: 
		{
			SingleVariable(ls);
			return;
		} break;
	default:
		FatalError("SyntaxError: Expected symbol");
	}
}

static void SuffixedExpression(LexerState* ls) {
	int line = ls->lineNumber;
	PrimaryExpression(ls);
	for (;;) {
		switch (ls->t.token) {
		case '(': case TK_VAR: // Function arguments
			{
				
			} break;
		}

	}
}

static void SingleVariable(LexerState* ls) {
	L_STRING* variableName = ls->t.semInfo.s;
}
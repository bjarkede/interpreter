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

	Exp  -> Exp + Exp2
	Exp  -> Exp - Exp2
	Exp  -> Exp2
	Exp2 -> Exp * Exp3
	Exp2 -> Exp / Exp3
	Exp2 -> Exp3
	Exp3 -> num
	Exp3 -> ( Exp )


   ----------- */

static const char* parseList[MAXSIZE];
static int parseIndex;

static void pushrule(const char* s) {
	parseList[parseIndex++] = s;
}

static void poprule(void) {
	--parseIndex;
}

static bool test_next(LexerState* ls, int c) {
	if (ls->t.token == c) {
		//next(ls);
		ProcessNextToken(ls);
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

void LL1(LexerState* ls) {

	parseIndex = 0;

	int table[NT_COUNT][TK_COUNT];
	memset(table, -1, sizeof(table));

	table[NT_EXP][TK_INT] = 0;
	table[NT_EXP][TK_SEPL] = 0;

	table[NT_EXP_][TK_ADD] = 1;
	table[NT_EXP_][TK_SUB] = 2;
	table[NT_EXP_][TK_SEPR] = 3;
	table[NT_EXP_][TK_EOZ] = 3;

	table[NT_EXP2][TK_INT] = 4;
	table[NT_EXP2][TK_SEPL] = 4;

	table[NT_EXP2_][TK_ADD] = 7;
	table[NT_EXP2_][TK_SUB] = 7;
	table[NT_EXP2_][TK_MUL] = 5;
	table[NT_EXP2_][TK_SUB] = 6;
	table[NT_EXP2_][TK_SEPR] = 7;
	table[NT_EXP2_][TK_EOZ] = 7;

	table[NT_EXP3][TK_INT] = 8;
	table[NT_EXP3][TK_SEPL] = 9;

	std::stack<int> stack;

	stack.push(TK_EOZ);
	stack.push(NT_EXP);
	
	ProcessNextToken(ls);

	/*while (!stack.empty()) {
		int t = stack.top();
		if (t == TK_ADD || t == TK_SUB || t == TK_MUL || t == TK_DIV || t == TK_INT || t == TK_SEPL || t == TK_SEPR || t == TK_EOZ) {
			PrintDebug("ParserAlert: Match found! Type: %d\n", ls->t.token);
			stack.pop();
			ProcessNextToken(ls);
		}
		else if (table[t][ls->t.token] == -1) {
			FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
		}
		else {
			switch (table[t][ls->t.token]) {
			case 0:
			{
				stack.pop();
				stack.push(NT_EXP_);
				stack.push(NT_EXP2);
			} break;
			case 1:
			{
				stack.pop();
				stack.push(NT_EXP_);
				stack.push(NT_EXP2);
				stack.push(TK_ADD);
			} break;
			case 2:
			{
				stack.pop();
				stack.push(NT_EXP_);
				stack.push(NT_EXP2);
				stack.push(TK_SUB);
			} break;
			case 3:
			{
				stack.pop();
			} break;
			case 4:
			{
				stack.pop();
				stack.push(NT_EXP2_);
				stack.push(NT_EXP3);
			} break;
			case 5:
			{
				stack.pop();
				stack.push(NT_EXP2_);
				stack.push(NT_EXP3);
				stack.push(TK_MUL);
			} break;
			case 6:
			{
				stack.pop();
				stack.push(NT_EXP2_);
				stack.push(NT_EXP3);
				stack.push(TK_DIV);
			} break;
			case 7:
			{
				stack.pop();
			} break;
			case 8:
			{
				stack.pop();
				stack.push(TK_INT);
			} break;
			case 9:
			{
				stack.pop();
				stack.push(TK_SEPL);
				stack.push(NT_EXP);
				stack.push(TK_SEPR);
			} break;
			}
		}
	}
	*/
}

static void PrimaryExpression(LexerState* ls) {
	if (ls->t.token == TK_INT || ls->t.token == TK_SEPL) {
		SecondaryExpression(ls);
		SuffixedExpression(ls);
	} else 
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
}

static void SecondaryExpression(LexerState* ls) {
	if (ls->t.token == TK_INT || ls->t.token == TK_SEPL) {
		SimpleExpression(ls);
		SuffixedSecondaryExpression(ls);
	} else
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
}	

static void SuffixedExpression(LexerState* ls) {
	if (ls->t.token == TK_ADD) {
		check_match(ls, TK_ADD, '+', ls->t.line);
		SecondaryExpression(ls);
		SuffixedExpression(ls);
	}
	else if (ls->t.token == TK_SUB) {
		check_match(ls, TK_SUB, '-', ls->t.line);
		SecondaryExpression(ls);
		SuffixedExpression(ls);
	}
	else if(ls->t.token == TK_SEPR || ls->t.token == TK_EOZ) {

	}
}

static void SuffixedSecondaryExpression(LexerState* ls) {
	if (ls->t.token == TK_ADD || ls->t.token == TK_SUB ||
		ls->t.token == TK_SEPR || ls->t.token == TK_EOZ) {
	}
	else if (ls->t.token == TK_MUL) {
		check_match(ls, TK_MUL, TK_MUL, ls->lookahead.line);
		SimpleExpression(ls);
		SuffixedSecondaryExpression(ls);
	}
	else if (ls->t.token == TK_DIV) {
		check_match(ls, TK_DIV, TK_DIV, ls->lookahead.line);
		SimpleExpression(ls);
		SuffixedSecondaryExpression(ls);
	} else
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
}

static void SimpleExpression(LexerState* ls) {
	if (ls->t.token == TK_INT) {
		//PrintDebug("Found literal %d\n", ls->t.semInfo.i);
		check_match(ls, TK_INT, 'n', ls->t.line);
	}
	else if (ls->t.token == TK_SEPL) {
		check_match(ls, TK_SEPL, TK_SEPL, ls->lookahead.line);
		PrimaryExpression(ls);
		check_match(ls, TK_SEPR, TK_SEPR, ls->lookahead.line);
	} else
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
}

static Exp* TerminalNode(RESERVED terminal) {
	switch (terminal) {
	case TK_INT: { return IntegerExp(0); } break;
	default:
		FatalError("SyntaxError: Failed when generating terminal expression.");
	}
}

// Takes a non-terminal symbol and a list of sub-trees that it uses to build
// the non-terminal node. 
static Exp* NonTerminalNode(RESERVED nonterminal, Exp** subtree) {
	switch (nonterminal) {
	case TK_ADD: 
	{
		return BinaryExp(Add, subtree[0], subtree[1]);
	} break;
	default:
		FatalError("SyntaxError: Failed when generating non-terminal expression");
	}
}

void Parse(LexerState* ls) {
	ProcessNextToken(ls);
	PrimaryExpression(ls);
	PrintDebug("SyntaxMessage: Input accepted by parser.\n");
}


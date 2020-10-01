#include "parser.hpp"
#include "interpreter.hpp"
#include "ast.hpp"

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

static bool test_next(LexerState* ls, int c) {
	if (ls->t.token == c) {
		ProcessNextToken(ls);
		return true;
	}
	else {
		return false;
	}
}

static bool test(LexerState* ls, int c) {
	if (ls->t.token == c) {
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

static bool check(LexerState* ls, int c) {
	if (ls->t.token != c)
		return false;
	return true;
}

static bool check_next(LexerState* ls, int c) {
	if (check(ls, c)) {
		ProcessNextToken(ls);
		return true;
	}
	return false;
}

void LL1(LexerState* ls) {
	int table[E_Count][TK_COUNT];
	memset(table, -1, sizeof(table));

	//table[E_Hole][TK_INT] = 0;

	table[E_BinOp][TK_ADD] = 1;
	table[E_BinOp][TK_SUB] = 2;
	table[E_BinOp][TK_EOZ] = 3;

	std::stack<Exp*> expStack;
	
	ProcessNextToken(ls);

	while (!expStack.empty()) {
		Exp* top = expStack.top();
		if (top->expType == E_Integer || top->expType == E_BinOp) {
			expStack.pop();

			switch (top->expType) {
			case E_Integer:
				{
				
				} break;
			case E_BinOp: 
				{
					
				} break;
			}

			ProcessNextToken(ls);
		}
		else if (table[top->expType][ls->t.token] == -1) {
			FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
		}
		else {
			Exp* nonTerminal = top; expStack.pop();
			
			switch (table[top->expType][ls->t.token]) {
			case 0: {} break;
			case 1: {} break;
			case 2: {} break;
			case 3: { expStack.pop(); } break; // return a hole that needs to be filled by the first int?
			}

		}
	}

	//PrintDebug("LL(1) Table-driven evaluation of output expression[ %s ]: %d.\n", toString(e).c_str(), eval(e).v.i);
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

	} else
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
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
		check_match(ls, TK_INT, 'n', ls->t.line);
	}
	else if (ls->t.token == TK_SEPL) {
		check_match(ls, TK_SEPL, TK_SEPL, ls->lookahead.line);
		PrimaryExpression(ls);
		check_match(ls, TK_SEPR, TK_SEPR, ls->lookahead.line);
	} else
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
}

static Exp* ParseExpression(LexerState* ls) {
	return ParseExpressionTernary(ls);
}

static bool IsUnaryOperator(LexerState* ls) {
	return test(ls, '+') || test(ls, '-') || test(ls, '*') || test(ls, '&');
}

static Exp* ParseExpressionBase(LexerState* ls) {
	Exp* expr = ParseExpressionOperand(ls);
	while (test(ls, '(') || test(ls, '[') || test(ls, '.')) {
		if (check_next(ls, '(')) {
			Exp** args = NULL;
			if (!test(ls, ')')) {
				// @TODO: Push arguments to args and parse them.
				// Do this while the next symbol is ','.
			}
			check_match(ls, ')', '(', ls->t.line);
		}
		else {

		}
	}
	return expr;
}

static Exp* ParseExpressionUnary(LexerState* ls) {
	if (IsUnaryOperator(ls)) {
		UnOpType op = GetUnaryOperator(ls->t.token);
		ProcessNextToken(ls);
		return UnaryExp(op, ParseExpressionUnary(ls));
	}
	else {
		return ParseExpressionBase(ls);
	}
}

static bool IsMulOperator(LexerState* ls) {
	return test(ls, '*') || test(ls, '/') || test(ls, '%') || test(ls, '&');
}

static Exp* ParseExpressionMul(LexerState* ls) {
	Exp* expr = ParseExpressionUnary(ls);
	while (IsMulOperator(ls)) {
		BinaryOpType op = GetBinaryOperator(ls->t.token);
		ProcessNextToken(ls);
		expr = BinaryExp(op, expr, ParseExpressionUnary(ls));
	}

	return expr;
}

static bool IsAddOperator(LexerState* ls) {
	return test(ls, '+') || test(ls, '-') || test(ls, '|') || test(ls, '^');
}

static Exp* ParseExpressionAdd(LexerState* ls) {
	Exp* expr = ParseExpressionMul(ls);
	while (IsAddOperator(ls)) {
		BinaryOpType op = GetBinaryOperator(ls->t.token);
		ProcessNextToken(ls);
		expr = BinaryExp(op, expr, ParseExpressionMul(ls));
	}
	return expr;
}

static bool IsCompOperator(LexerState* ls) {
	return test(ls, '<') || test(ls, '>') || test(ls, TK_EQ) || test(ls, TK_LE);
}

static Exp* ParseExpressionCompare(LexerState* ls) {
	Exp* expr = ParseExpressionAdd(ls);
	while (IsCompOperator(ls)) {
		BinaryOpType op = GetBinaryOperator(ls->t.token);
		ProcessNextToken(ls);
		return BinaryExp(op, expr, ParseExpressionAdd(ls));
	}
	return expr;
}

static Exp* ParseExpressionAnd(LexerState* ls) {
	Exp* expr = ParseExpressionCompare(ls);
	while (check_next(ls, TK_AND)) {
		expr = BinaryExp(And, expr, ParseExpressionCompare(ls));
	}
	return expr;
}

static Exp* ParseExpressionOr(LexerState* ls) {
	Exp* expr = ParseExpressionAnd(ls);
	while (check_next(ls, TK_OR)) {
		expr = BinaryExp(Or, expr, ParseExpressionAnd(ls));
	}
	return expr;
}

static Exp* ParseExpressionTernary(LexerState* ls) {
	Exp* expr = ParseExpressionOr(ls);
	
	// @TODO:

	return expr;
}

static Exp* ParseParenthesisedExpression(LexerState* ls) {
	check(ls, '(');
	Exp* expr = ParseExpression(ls);
	check(ls, ')');
	return expr;
}

static UnOpType GetUnaryOperator(int op) {
	switch (op) {
	case '!': return Not;
	case '-': return Minus;
	case '~': return BNot;
	case '#': return Len;
	default:
		return NoUnOp;
	}
}

static BinaryOpType GetBinaryOperator(int op) {
	switch (op) {
	case '+': return Add;
	case '-': return Sub;
	case '*': return Mul;
	case '%': return Mod;
	case '^': return Pow;
	case '/': return Div;
	default:
		return NoBinOp;
	}
}

Exp* ParseExpressionOperand(LexerState*ls) {
	switch (ls->t.token) {
	case TK_INT: { unsigned long long val = ls->t.semInfo.i; ProcessNextToken(ls); return IntegerExp(val); } break;
	case TK_VAR: { L_STRING* val = ls->t.semInfo.s; ProcessNextToken(ls); return VariableExp(val); } break;
	case '(': {
		check_match(ls, '(', '(', ls->t.line);
		Exp* expr = ParseExpression(ls);
		check_match(ls, ')', ')', ls->t.line);
		return ParenExp(expr);
	} break;
	case TK_LET: {
		// Expr -> let VAR = Expr in Expr end 
		check_match(ls, TK_LET, TK_LET, ls->t.line);
		Exp* var = ParseExpression(ls);
		if(var->expType != E_Variable) 
			FatalError("SyntaxError: Expected variable but got [ %s ], Line: %d[%d ].", toString(var).c_str(), ls->lastLine, ls->lastCol);
		check_match(ls, '=', '=', ls->t.line);
		Exp* binding = ParseExpression(ls);
		check_match(ls, TK_IN, TK_IN, ls->t.line);
		Exp* expr = ParseExpression(ls);
		check_match(ls, TK_END, TK_END, ls->t.line);
		return LetExp(var, binding, expr);
	} break;
	default:
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
	}
}

void Parse(LexerState* ls) {

	ProcessNextToken(ls);
	Exp* expr = ParseExpression(ls);

	PrintDebug("SyntaxMessage: Input accepted by parser.\n\n");
	PrintDebug("Testing AST Interpretation:\n");
	PrintDebug("Expression: [ %s ]\n       Value: %d\n\n", toString(expr).c_str(), eval(expr).v);
}


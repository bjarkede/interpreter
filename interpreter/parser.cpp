#include "parser.hpp"
#include "ast.hpp"

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

Exp* ParseExpression(LexerState* ls) {
	return ParseExpressionTernary(ls);
}

static bool IsUnaryOperator(LexerState* ls) {
	return test(ls, '+') || test(ls, '-') || test(ls, '*') || test(ls, '&');
}

static Exp* ParseExpressionBase(LexerState* ls) {
	Exp* expr = ParseExpressionOperand(ls);
	int len = 0;
	while (test(ls, '(') || test(ls, '[') || test(ls, '.')) {
		if (check_next(ls, '(')) {
			Buffer args;
			AllocBuffer(args, sizeof(Expression*) * 5);
			if (!test(ls, ')')) {
				// @TODO: Push arguments to args and parse them.
				// Do this while the next symbol is ','.
				//args[len++] = ParseExpression(ls);
				((Exp**)args.buffer)[args.writeIndex++] = ParseExpression(ls);
				while (check_next(ls, ',')) {
					((Exp**)args.buffer)[args.writeIndex++] = ParseExpression(ls);
				}
			}
			check_match(ls, ')', '(', ls->t.line);
			expr = CallExp(expr, args);
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
		// 
		check_match(ls, TK_LET, TK_LET, ls->t.line);
		Exp* e = ParseExpression(ls);
	
		switch (e->expType) {
		case E_Variable: {
			check_match(ls, '=', '=', ls->t.line);
			Exp* binding = ParseExpression(ls);
			check_match(ls, TK_IN, TK_IN, ls->t.line);
			Exp* expr = ParseExpression(ls);
			check_match(ls, TK_END, TK_END, ls->t.line);
			return LetExp(e, binding, expr);
		} break;
		case E_Call: {
			check_match(ls, '=', '=', ls->t.line);
			Exp* fbody = ParseExpression(ls);
			check_match(ls, TK_IN, TK_IN, ls->t.line);
			Exp* letbody = ParseExpression(ls);
			check_match(ls, TK_END, TK_END, ls->t.line);
			return LetFunExp(((Var*)((Call*)e)->eFun)->name, ((Call*)e)->args, fbody, letbody);
		} break;
		default:
			FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
		}
	} break;
	default:
		FatalError("SyntaxError: Invalid input: %c, Line: %d[%d ].", (char)ls->t.token, ls->t.line, ls->t.col);
	}
}

void Parse(LexerState* ls) {
	/*
	std::vector<Expression*> expList;
	// @TODO:
	// Parse multiple expressions.
	ProcessNextToken(ls);
	while (ls->t.token != TK_EOZ) {
		expList.push_back(ParseExpression(ls));
	}

	PrintDebug("SyntaxMessage: Input accepted by parser.\n\n");
	PrintDebug("Testing AST Interpretation:\n");

	for (auto& e : expList) {
		PrintDebug("Expression: [ %s ]\n       Value: %d\n\n", toString(e).c_str(), eval(e).v);
	}
	*/
	
}


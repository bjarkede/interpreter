#include "ast.hpp"

Expression* EmptyExp() {
	Expression* e = (Expression*)malloc(sizeof(Expression));
	e->expType = E_NoType;

	return e;
}

Expression* StringExp(L_STRING* val) {
	String* e = (String*)malloc(sizeof(String));
	e->expType = E_String;
	e->value = val->contents;

	return e;
}

Expression* IntegerExp(L_INTEGER i) {
	Integer* e = (Integer*)malloc(sizeof(Integer));

	e->expType = E_Integer;
	e->value = i;

	return e;
}

Expression* BinaryExp(BinaryOpType op, Expression* left, Expression* right) {
	BinOp* e = (BinOp*)malloc(sizeof(BinOp));

	e->expType = E_BinOp;
	e->opType = op;
	e->left = left;
	e->right = right;

	return e;
}

Expression* UnaryExp(UnOpType op, Expression* expr) {
	UnOp* e = (UnOp*)malloc(sizeof(UnOp));

	e->expType = E_UnOp;
	e->opType = op;
	e->expr = expr;

	return e;
}


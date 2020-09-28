#include "ast.hpp"

Expression* EmptyExp() {
	Expression* e = (Expression*)malloc(sizeof(Expression));
	e->expType = E_NoType;

	return e;
}

Expression* IntegerExp(int i) {
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


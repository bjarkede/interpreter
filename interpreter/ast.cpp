#include "ast.hpp"

AST_Node* IntegerExp(int i) {
	AST_Node* e = (AST_Node*)malloc(sizeof(AST_Node));
	e->tag = AST_Node::Type::IntegerExp;
	e->operation.value.i = i;
	return e;
}

AST_Node* BinaryExp(char op, AST_Node* left, AST_Node* right) {
	AST_Node* e = (AST_Node*)malloc(sizeof(AST_Node));
	
	e->tag = AST_Node::Type::BinExp;

	e->operation.BinExp.op = op;
	e->operation.BinExp.left = left;
	e->operation.BinExp.right = right;	

	return e;
}

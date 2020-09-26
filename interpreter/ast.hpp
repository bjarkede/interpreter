#ifndef AST_HPP
#define AST_HPP

#include "local.hpp"

enum BinaryOpType {
	Add,
	Mul,
	Sub,
	OP_Count
};

enum ExpressionType
{
	E_Integer,
	E_BinOp,
	E_UnOp,
	E_Count
};

struct Expression {
	ExpressionType expType;
};

typedef struct Integer : public Expression {
	int value;
} Integer;

typedef struct BinOp : public Expression {
	BinaryOpType opType;
	Expression* left;
	Expression* right;
} BinOp;

/*typedef struct AST_List {
	struct AST_Node* element;
	struct AST_List* next;
} AST_List ;*/


// Literals
Expression* IntegerExp(int i);

// Binary Expressions
Expression* BinaryExp(BinaryOpType op, Expression* left, Expression* right);

#endif
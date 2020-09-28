#ifndef AST_HPP
#define AST_HPP

#include "local.hpp"

enum BinaryOpType {
	// Arithmetic
	Add, Sub, Mul, Mod, Pow,
	Div, IDiv,
	// Comparison
	EQ, LT,
	// Logical
	AND, OR,
	// Others
	MIN, MAX,

	OP_NOBINOPR,
	OP_COUNT
};

enum ExpressionType
{
	E_Integer,
	E_BinOp,
	E_UnOp,
	E_NoType,
	E_Count
};

typedef struct Expression {
	ExpressionType expType;
} Exp;

typedef struct Integer : public Expression {
	int value;
} Integer;

typedef struct BinOp : public Expression {
	BinaryOpType opType;
	Expression* left;
	Expression* right;
} BinOp;

// Empty
Expression* EmptyExp();

// Literals
Expression* IntegerExp(int i);

// Binary Expressions
Expression* BinaryExp(BinaryOpType op, Expression* left, Expression* right);

#endif